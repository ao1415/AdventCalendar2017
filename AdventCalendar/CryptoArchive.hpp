#pragma once
#include <Siv3D.hpp>
#include <queue>

namespace CryptoArchive
{
	/// <summary>
	/// �t�@�C���A�[�J�C�u���쐬���܂��B
	/// </summary>
	/// <param name="from">
	/// �A�[�J�C�u������f�B���N�g��
	/// </param>
	/// <param name="to">
	/// �ۑ�����A�[�J�C�u�t�@�C����
	/// </param>
	/// <returns>
	/// �t�@�C���A�[�J�C�u�̍쐬�ɐ��������ꍇ true, ����ȊO�̏ꍇ�� false
	/// </returns>
	bool Create(const FilePath& from, const FilePath& to, const AES128Key& key) {

		/// <summary>��Ɨp�̃t�H���_�̖��O</summary>
		const String WorkFolder = L"./encrypt-archive_working-folder/";

		/// <summary>�����t���O</summary>
		bool success = true;

		if (FileSystem::IsDirectory(from))
		{
			//��Ɨp�̃t�H���_�̍쐬
			success &= FileSystem::CreateDirectories(WorkFolder);

			//��ƃt�H���_�̃p�X����
			const FilePath BaseFolder = WorkFolder + FileSystem::BaseName(from);
			//�Ώۃt�H���_����ƃt�H���_�փR�s�[
			success &= FileSystem::Copy(from, BaseFolder);

			//��ƃt�H���_�ȉ��̃t�@�C�������ׂĈÍ�������
			std::queue<FilePath> files;
			files.push(BaseFolder);

			while (!files.empty())
			{
				const auto& filePath = files.front();

				if (FileSystem::IsFile(filePath))
				{
					success &= Crypto2::EncryptFile(filePath, filePath, key);
				}
				else if (FileSystem::IsDirectory(filePath))
				{
					const auto& contents = FileSystem::DirectoryContents(filePath);
					for (const auto& path : contents)
					{
						files.push(path);
					}
				}

				files.pop();
			}

			//��ƃt�H���_���A�[�J�C�u�����ĕۑ�����
			success &= Archive::Create(BaseFolder, to);

			//��Ɨp�̃t�H���_���폜����
			success &= FileSystem::Remove(WorkFolder, false);
		}

		return success;
	}
}

/// <summary>
/// �Í����t�@�C���̃A�[�J�C�u
/// </summary>
class ArchiveCryptoFile
{
private:

	/// <summary>�t�@�C���A�[�J�C�u</summary>
	FileArchive fileArchive;

	/// <summary>�A�[�J�C�u�����邩�̃t���O</summary>
	bool archive = false;

	AES128Key key;

	/// <summary>
	/// �Í����t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <returns>�������f�[�^</returns>
	Optional<ByteArray> loadFile(const String path)
	{
		//���̂܂ܓǂݍ���Ńf�[�^��Ԃ�
		ByteArray data(path);

		return Optional<ByteArray>(data);
	}
	/// <summary>
	/// �A�[�J�C�u���̈Í����t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <returns>�������f�[�^</returns>
	Optional<ByteArray> loadArchiveFile(const String path)
	{
		//���������ăf�[�^��Ԃ�
		auto reader = fileArchive.load(path);
		Array<uint8> buf(static_cast<unsigned int>(reader.size()));
		reader.read(buf.data(), buf.size());

		return Crypto2::Decrypt(ByteArray(std::move(buf)), key);
	}

public:

	/// <summary>
	/// �ʏ�t�@�C���p�R���X�g���N�^
	/// </summary>
	ArchiveCryptoFile()
	{
		archive = false;
	}

	/// <summary>
	/// �A�[�J�C�u�t�@�C���p�R���X�g���N�^
	/// </summary>
	/// <param name="archivePath">�A�[�J�C�u�t�@�C���p�X</param>
	/// <param name="key">������</param>
	ArchiveCryptoFile(const String& archivePath, const AES128Key& key)
	{
		archive = true;
		this->key = key;

		fileArchive.open(archivePath);
	}

	/// <summary>
	/// �t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>�t�@�C���f�[�^</returns>
	ByteArray load(const String& filePath) {

		if (archive)
		{
			String path = filePath;
			if (filePath.indexOf(L"./") == 0)
				path = path.erase(0, 2);

			const auto& file = loadArchiveFile(path);
			if (file)
				return std::move(file.value());
		}
		else
		{
			const auto& file = loadFile(filePath);
			if (file)
				return std::move(file.value());
		}

		return ByteArray();
	}

};
