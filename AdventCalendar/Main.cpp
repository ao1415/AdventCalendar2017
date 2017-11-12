#include <Siv3D.hpp>

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

		//TODO:フォルダ内のファイルは全て暗号化する
		Crypto2::EncryptFile(from, to, key);

		return false;
	}
}

class CryptoFile
{
private:

	FileArchive fileArchive;
	bool archive = false;

	AES128Key key;

	Optional<ByteArray> loadFile(const String path)
	{
		ByteArray data(path);

		return Crypto2::Decrypt(data, key);
	}
	Optional<ByteArray> loadArchiveFile(const String path)
	{
		auto reader = fileArchive.load(path);
		Array<uint8> buf(static_cast<unsigned int>(reader.size()));
		reader.read(buf.data(), buf.size());

		return Crypto2::Decrypt(ByteArray(std::move(buf)), key);
	}

public:

	CryptoFile(const AES128Key& key)
	{
		archive = false;
		this->key = key;
	}

	CryptoFile(const String& archivePath, const AES128Key& key)
	{
		archive = true;
		this->key = key;

		fileArchive.open(archivePath);
	}

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

void Main()
{
	Console::Open();

	//CryptoFile file(AES128Key(0, 0));
	CryptoFile file(L"./archive.ecp", AES128Key(0, 0));
	Texture texture(file.load(L"./archive/Siv3D-kun.ecp"));

	texture.draw();

	System::Update();
	WaitKey();
}
