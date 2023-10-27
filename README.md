# Next Micon Package Registory

## パッケージの追加方法

パッケージを自作し NextMicon で使用する方法

### 1. プロジェクト・ローカル

プロジェクトの `.package` ディレクトリ以下に、パッケージを置くことで、プロジェクトで使用されるパッケージとして認識されます。

```
project
|- .package
   |- MyPackage
      |- nmpackage.json
```

### 2. GitHubで公開

パッケージは、GitHubを用いて公開できます。

まず、パッケージを置くためのリポジトリを作成します。ここでは、例として `my-registory` とします。

次に、作成したリポジトリに、`packages.json` を設置します。
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

Next Micon IDE の設定画面から、`packages.json` の場所を追加します。
