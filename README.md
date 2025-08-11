# ESP8266
基于nsp8266的网页控制的无线控制船模（竞速）的无线控制模块（单无刷电机，单电调）
🚤 ESP8266 Web-Controlled RC Boat

📖 项目简介

本项目基于 ESP8266 开发板，实现通过网页实时控制 单无刷电机（BLDC + ESC） 和 单舵机 的 RC 船。
用户可以通过手机或电脑连接船模的 Wi-Fi 热点，打开网页即可进行油门和转向控制。
支持：

安全开关（防止上电电机突然转动）

虚拟摇杆（手机横屏控制更方便）

低延迟 WebSocket 实时控制


🛠 硬件清单

组件	型号/说明	数量

主控	ESP8266 NodeMCU / Wemos D1 mini	1
无刷电机	RS2205 / 2300KV 等航模电机	1
电调（ESC）	12V-24V 三相无刷电机调速器（带 BEC 输出）	1
舵机	SG90 / MG90S / MG996R 等	1
电池	2S/3S LiPo	1
其他	杜邦线、面包板或焊接线材、防水船体	-



---

🔌 接线图



> 注意：ESC 信号线连接到 D1，舵机信号线连接到 D2，所有 GND 必须共地。




---

📄 软件功能

Wi-Fi AP 模式（默认 SSID: BoatControl / 密码: 12345678）

内置网页服务器（访问 192.168.4.1）

WebSocket 实时控制

安全开关（Arm/Disarm）

虚拟摇杆（左舵机，右油门）



---

📂 文件结构

├── src

│   └── main.ino         # 主程序代码

├── docs

│   └── wiring.png       # 接线图

├── LICENSE              # 许可证

└── README.md            # 项目说明文档



---

⚙️ 使用方法

1. 下载代码

git clone https://github.com/yourname/esp8266-rc-boat.git


2. 安装 Arduino IDE & ESP8266 Core

在 Arduino IDE 中添加开发板管理 URL：

http://arduino.esp8266.com/stable/package_esp8266com_index.json

安装 “ESP8266 Boards”



3. 接线（参考接线图）


4. 烧录固件


5. 连接 Wi-Fi

连接 SSID: BoatControl

密码: 12345678



6. 打开浏览器

输入 192.168.4.1 访问网页控制界面





---

---

🛡 安全说明

上电时请保持安全开关关闭（绿色）

测试时务必固定电机，防止飞转伤人

船体测试时注意防水与电池防护



---

📜 许可证

本项目基于 MIT License 开源，详情见 LICENSE 文件。


---

🤝 贡献

欢迎提交 Pull Request 或 Issue 来改进本项目。
你可以：

优化网页界面

增加传感器（GPS、电压检测等）

优化油门控制算法

