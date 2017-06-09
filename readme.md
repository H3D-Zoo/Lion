# Render API

这是一个渲染API抽象

## 目录结构

* RenderAPI 接口定义
* Source 接口实现
 * source\DX9API 标准渲染实现，用dx9实现
 * source\FixedAPI 固定管线渲染实现，用dx9兼容模式实现
 * source\NullAPI 空的接口实现，用来保证无作用的运行
* build 通过 gen_vc_sln.bat 使用 cmake 生成的项目文件目录
* bin render.dll生成目录
* gen_vc_sln.bat 这个bat会直接使用cmake生成工程文件