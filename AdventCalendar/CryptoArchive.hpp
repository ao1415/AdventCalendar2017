#pragma once
#include <Siv3D.hpp>

/// <summary>
/// 暗号化ファイルのアーカイブ
/// </summary>
class ArchiveCryptoFile
{
private:

	FileArchive fileArchive;
	bool archive = false;

	AES128Key key;

	/// <summary>
	/// 暗号化ファイルの読み込み
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>復号化データ</returns>
	Optional<ByteArray> loadFile(const String path)
	{
		ByteArray data(path);

		return Crypto2::Decrypt(data, key);
	}
	/// <summary>
	/// アーカイブ中の暗号化ファイルの読み込み
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>復号化データ</returns>
	Optional<ByteArray> loadArchiveFile(const String path)
	{
		auto reader = fileArchive.load(path);
		Array<uint8> buf(static_cast<unsigned int>(reader.size()));
		reader.read(buf.data(), buf.size());

		return Crypto2::Decrypt(ByteArray(std::move(buf)), key);
	}

public:

	/// <summary>
	/// 通常ファイル用コンストラクタ
	/// </summary>
	/// <param name="key">復号鍵</param>
	ArchiveCryptoFile(const AES128Key& key)
	{
		archive = false;
		this->key = key;
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
