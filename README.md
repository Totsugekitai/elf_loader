# elf_loader

情報特別演習2で作成している自作簡易ローダのリポジトリです。

C++で書く予定です。

## 動的リンクまでの道のり

とりあえず「リンカ・ローダ実践開発テクニック」を読んで情報をまとめる。

- `.symtab`と`strtab`セクションを取得
- 上記のテーブルから再配置テーブルである`.rela.XXXX`があったら再配置する
  - シンボルがundefinedなら、読み込んだ全オブジェクトファイルからシンボルを検索してシンボルの実体が格納されているアドレスを取得
  - シンボル解決するアドレスを取得する
    - SHT_RELAの場合は`r_addend`を加算する必要あり
  - 実際にアドレスを書き込みする
