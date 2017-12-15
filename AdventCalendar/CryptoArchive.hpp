#pragma once
#include <Siv3D.hpp>
#include <queue>

namespace CryptoArchive
{
	/// <summary>
	/// ファイルアーカイブを作成します。
	/// </summary>
	/// <param name="from">
	/// アーカイブ化するディレクトリ
	/// </param>
	/// <param name="to">
	/// 保存するアーカイブファイル名
	/// </param>
	/// <returns>
	/// ファイルアーカイブの作成に成功した場合 true, それ以外の場合は false
	/// </returns>
	bool Create(const FilePath& from, const FilePath& to, const AES128Key& key) {

		/// <summary>作業用のフォルダの名前</summary>
		const String WorkFolder = L"./encrypt-archive_working-folder/";

		/// <summary>成功フラグ</summary>
		bool success = true;

		if (FileSystem::IsDirectory(from))
		{
			//作業用のフォルダの作成
			success &= FileSystem::CreateDirectories(WorkFolder);

			//作業フォルダのパス生成
			const FilePath BaseFolder = WorkFolder + FileSystem::BaseName(from);
			//対象フォルダを作業フォルダへコピー
			success &= FileSystem::Copy(from, BaseFolder);

			//作業フォルダ以下のファイルをすべて暗号化する
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

			//作業フォルダをアーカイブ化して保存する
			success &= Archive::Create(BaseFolder, to);

			//作業用のフォルダを削除する
			success &= FileSystem::Remove(WorkFolder, false);
		}

		return success;
	}
}

/// <summary>
/// 暗号化ファイルのアーカイブ
/// </summary>
class ArchiveCryptoFile
{
private:

	/// <summary>ファイルアーカイブ</summary>
	FileArchive fileArchive;

	/// <summary>アーカイブ化するかのフラグ</summary>
	bool archive = false;

	AES128Key key;

	/// <summary>
	/// 暗号化ファイルの読み込み
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>復号化データ</returns>
	Optional<ByteArray> loadFile(const String path)
	{
		//そのまま読み込んでデータを返す
		ByteArray data(path);

		return Optional<ByteArray>(data);
	}
	/// <summary>
	/// アーカイブ中の暗号化ファイルの読み込み
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>復号化データ</returns>
	Optional<ByteArray> loadArchiveFile(const String path)
	{
		//復号化してデータを返す
		auto reader = fileArchive.load(path);
		Array<uint8> buf(static_cast<unsigned int>(reader.size()));
		reader.read(buf.data(), buf.size());

		return Crypto2::Decrypt(ByteArray(std::move(buf)), key);
	}

public:

	/// <summary>
	/// 通常ファイル用コンストラクタ
	/// </summary>
	ArchiveCryptoFile()
	{
		archive = false;
	}

	/// <summary>
	/// アーカイブファイル用コンストラクタ
	/// </summary>
	/// <param name="archivePath">アーカイブファイルパス</param>
	/// <param name="key">復号鍵</param>
	ArchiveCryptoFile(const String& archivePath, const AES128Key& key)
	{
		archive = true;
		this->key = key;

		fileArchive.open(archivePath);
	}

	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>ファイルデータ</returns>
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
