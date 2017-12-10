#include <Siv3D.hpp>
#include "CryptoArchive.hpp"

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

void Main()
{

	//ArchiveCryptoFile file(AES128Key(0, 0));
	ArchiveCryptoFile file(L"./archive.ecp", AES128Key(0, 0));
	Texture texture(file.load(L"./archive/Siv3D-kun.ecp"));

	texture.draw();

	System::Update();
	WaitKey();
}
