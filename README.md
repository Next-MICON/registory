# Next Micon Registory

## パッケージの作成

パッケージを自作し NextMicon で使用する方法を説明します。

- `${owner}` はパッケージの作成者名を指します。あなたの github のアカウント名に置き換えてください。
- `${package}` はパッケージの名前を指します。あなたが作るパッケージ名に置き換えてください。
- `${version}` はパッケージのバージョンを指します。最初は `0.0.0` にしてください。

`~/NextMiconIDE/Package/${owner}/${package}/${version}` というディレクトリを作ります。

このディレクトリ下に以下の 4 つのファイルを作成します。

- `package.nm.yaml`
- `${package}.cpp`
- `${package}.hpp`
- `${package}.sv`
