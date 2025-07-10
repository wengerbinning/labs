
## 开发环境

### WSL debian

```
sudo apt install libcups2
sudo apt install npm libnss3 libatk1.0-0 libatk-bridge2.0-0
sudo apt install libgtkd-3-0 libasound2
```

### ArchLinux

##  项目开发

```shell
npm init
```

### 项目环境

```shell
# npm config set registry https://mirrors.huaweicloud.com/repository/npm/
npm install --save-dev electron
```

#### 开发调试

```shell
npm run start
```

### 发布版本

#### 准备环境

```shell
npm install --save-dev @electron-forge/cli
```

```shell
npx electron-forge import
```

### 发行程序


```shell
npm run make
```