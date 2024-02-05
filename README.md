# Next Micon Registory <!-- omit in toc -->

## Install ALL Packages & Boards at first

```
$ cd
$ git clone git@github.com:NextMicon/Registory.git NextMiconIDE
```

## パッケージの作成

パッケージを自作し NextMicon で使用する方法を説明します。

- `${owner}` はパッケージの作成者名を指します。あなたの github のアカウント名に置き換えてください。
- `${package}` はパッケージの名前を指します。あなたが作るパッケージ名に置き換えてください。
- `${version}` はパッケージのバージョンを指します。最初は `0.0.0` にしてください。

### 1.1. ファイルの作成

`NextMiconIDE/Package/${owner}/${package}/${version}` というディレクトリを作ります。

このディレクトリ下に以下の 4 つのファイルを作成します。

- `package.nm.yaml`
- `${package}.cpp`
- `${package}.hpp`
- `${package}.sv`

### 1.2. package.nm.yaml

```
ports:
  - name: tx
    direct: output
    width: 1
    pos: [120, -20]
  - name: rx
    direct: input
    width: 1
    pos: [120, 20]
params: []
size: [240, 80]
textX: -50
software:
  className: ${package}
  memSize: 0x10
  member:
    - doc: "baud(int)"
      copy: "${INSTANCE}.baud()"
    - doc: "print(char)"
      copy: "${INSTANCE}.print()"
    - doc: "print(const char*)"
      copy: "${INSTANCE}.print()"
    - doc: "print(uint32_t)"
      copy: "${INSTANCE}.print()"
    - doc: "read() -> char"
      copy: "${INSTANCE}.read()"
```

### 1.3. System Verilog

```
module #{package} (
    input wire clk,
    input wire resetn,

    input wire valid,
    output wire ready,
    input wire [3:0] wstrb,
    input wire [31:0] addr,
    input wire [31:0] wdata,
    output wire [31:0] rdata,

    output tx,
    input  rx
);


endmodule
```

### 1.4. C++

### 1.5. Publish on GitHub

`NextMicon/Registory` にプルリクエストを出してください。
