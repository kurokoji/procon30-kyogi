# solver

## 依存ライブラリ

- Boost 1.61.0 or later

## ビルド

CMakeを必ずインストールすること．
また，Ninjaを使えばビルドが速くなるのでインストール推奨

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

or

```
mkdir build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
make
```
