# Next Micon Registory <!-- omit in toc -->

## Install ALL Packages & Boards at first

```
$ cd
$ git clone git@github.com:NextMicon/Registory.git NextMiconIDE
```

## パッケージの作成

パッケージを自作し NextMicon で使用する方法を説明します。

`${owner}` はパッケージの作成者名を指します。あなたのgithubのアカウント名に置き換えてください。

`${package}` はパッケージの名前を指します。あなたが作るパッケージ名に置き換えてください。

`${version}` はパッケージのバージョンを指します。最初は `0.0.0` にしてください。

### 1.1. パッケージの作成

`NextMiconIDE/Package/${owner}/${name}/${version}` 

プロジェクトの `.package` ディレクトリ以下に、パッケージを置くことで、プロジェクトで使用されるパッケージとして認識されます。

```
project
|- .package
   |- MyPackage
      |- nmpackage.json
```

### 1.2. GitHubで公開

パッケージは、GitHubを用いて公開できます。

## バージョン番号について

公開したパッケージを変更する場合、必ずバージョン番号をインクリメントしてください。その際に、セマンティックバージョニングに従ってください。

セマンティックバージョニングは、パブリックAPIの互換性の基づいて定義されています。
NextMicon のパッケージにおけるパブリックAPIとは、以下が該当します。

- ハードウェアの入出力ポート
- ハードウェアのパラメタ
- ソフトウェアのメンバ関数

NextMicon におけるセマンティックバージョニングは、具体的には以下のようになります。

**メジャー**

`x.y.z` → `(x+1).0.0` の変更は、後方互換性のない変更です。
`x.y.z` のインスタンスを `(x+1).y.z` に変更した場合、プロジェクトはビルドできません。
具体的には以下の場合が該当します。

- 入出力ポートの削除
- パラメタの削除

**マイナー**

`x.y.z` → `x.(y+1).0` の変更は、後方互換性のある変更です。
`x.y.z` のインスタンスを `x.(y+1).0` に変更した場合、プロジェクトはビルドできますが、
機能的に異なっているため、ハードウェアリソースの消費量および動作速度は変化する場合があります。

**パッチ**

`x.y.z` → `x.y.(z+1)` の変更は、後方互換性のある小さな変更です。
バージョン番号をアップデートしても、そのまま動きます。

**プレリリース**

#### 1.2.1. リポジトリの作成

パッケージを置くためのリポジトリを作成します。
ここでは、例として `my-registory` とします。

作成したリポジトリに、`registory.json` を設置します。
IDE がレジストリを探索するときのエントリポイントになります。

```json:
{
    "owner": "your-github-id",
    "repo": "my-registory",
    "branch": "main",
    "packages": [
        {"name":"MyPackage", "versions": ["0.0.0"], "category": "SignalIO"}
    ]
}
```

`owner`, `repo`, `branch` は GitHub 上でのレジストリの場所を示します。
レジストリのルートディレクトリが以下のURLにあることを表しています。

`https://raw.github.com/${owner}/${repo}/${branch}/registory.json`

#### 1.2.2. パッケージの追加

`packages` 以下に、レジストリに含まれるパッケージを記述します。
`name`と`version`から、パッケージの場所を取得します。

`https://raw.github.com/${owner}/${repo}/${branch}/${name}/${version}/nmpackage.json`


リポジトリの構造は

```
my-registory
|- packages.json
|- MyPackage
   |- 0.0.0
      |- package.json
      |- MyPackage.v
      |- MyPackage.hpp
      |- MyPackage.cpp
```

#### 1.2.3. IDEへの登録

Next Micon IDE の設定画面から、`registory.json` の場所を追加します。

公式リポジトリのURLは以下の通りになります。

```
https://raw.github.com/NextMicon/registory/main/registory.json
```

IDE側から、レジストリが認識されるようになりました。

IDEを再起動してください。

#### 1.2.4. 注意

バージョン番号は[セマンティック・バージョニング](https://semver.org/)を守るようにしてください。

また、できる限りリポジトリを削除しないようにしてください。
