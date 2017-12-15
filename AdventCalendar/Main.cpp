#include <Siv3D.hpp>
#include "CryptoArchive.hpp"

void Main()
{
	//フォルダ内のファイルを丸ごと暗号化して、アーカイブ化する
	CryptoArchive::Create(L"./Example", L"./Example.ecp", AES128Key(0, 0));

	//通常ファイルの読み込み
	//ArchiveCryptoFile file;

	//アーカイブファイルの読み込み
	ArchiveCryptoFile file(L"./Example.ecp", AES128Key(0, 0));

	//読み込む際にはどちらでも同じように記載できる
	Texture texture(file.load(L"./Example/Siv3D-kun.png"));

	while (System::Update())
	{
		texture.draw();
	}

}
