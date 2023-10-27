# Next Micon Package Registory <!-- omit in toc -->

## 1. パッケージの追加方法

パッケージを自作し NextMicon で使用する方法

### 1.1. パッケージの作成

プロジェクトの `.package` ディレクトリ以下に、パッケージを置くことで、プロジェクトで使用されるパッケージとして認識されます。

```
project
|- .package
   |- MyPackage
      |- nmpackage.json
```

### 1.2. GitHubで公開

パッケージは、GitHubを用いて公開できます。

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
