#pragma once
#include <Siv3D.hpp>

/// <summary>
/// �Í����t�@�C���̃A�[�J�C�u
/// </summary>
class ArchiveCryptoFile
{
private:

	FileArchive fileArchive;
	bool archive = false;

	AES128Key key;

	/// <summary>
	/// �Í����t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <returns>�������f�[�^</returns>
	Optional<ByteArray> loadFile(const String path)
	{
		ByteArray data(path);

		return Crypto2::Decrypt(data, key);
	}
	/// <summary>
	/// �A�[�J�C�u���̈Í����t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <returns>�������f�[�^</returns>
	Optional<ByteArray> loadArchiveFile(const String path)
	{
		auto reader = fileArchive.load(path);
		Array<uint8> buf(static_cast<unsigned int>(reader.size()));
		reader.read(buf.data(), buf.size());

		return Crypto2::Decrypt(ByteArray(std::move(buf)), key);
	}

public:

	/// <summary>
	/// �ʏ�t�@�C���p�R���X�g���N�^
	/// </summary>
	/// <param name="key">������</param>
	ArchiveCryptoFile(const AES128Key& key)
	{
		archive = false;
		this->key = key;
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
