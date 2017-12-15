# 暗号化ファイルアーカイブ
### このリポジトリは[Siv3D AdventCalendar 2017](https://qiita.com/advent-calendar/2017/siv3d)用に作成されました。


暗号化ファイルのアーカイブからデータを読み込む機能です。

動作環境
- Visual Studio 2015
- Siv3D(August2016v2)

[CryptoArchive.hpp](https://github.com/ao1415/AdventCalendar2017/blob/master/AdventCalendar/CryptoArchive.hpp)を読み込むことで、使用できるようになります。

## 機能説明
### CryptoArchive::Create
指定したフォルダをアーカイブ化します。  
指定したフォルダ内のファイルはすべて、指定した鍵で暗号化されます。  
作成したアーカイブ内のフォルダとファイル構成は、元のフォルダとファイル構成の通りになります。  

### ArchiveCryptoFile
アーカイブ化されたファイルから、データを読み込む機能です。  
コンストラクタで、読み込むアーカイブファイルと鍵を指定します。  
データを読み込む際には load でファイルパスを指定します。  

### 注意
コンストラクタで何も指定しない場合、指定されたファイルパスにあるファイルを読み込みます。  
