#include <ESP8266WiFi.h>         // ESP8266 Wi-Fi 功能
#include <ESP8266WebServer.h>    // 简易 HTTP 服务器
#include <WebSocketsServer.h>    // WebSocket 实时通信
#include <Servo.h>               // 舵机/电调 PWM 控制

const char* ssid="BoatControl";   // Wi-Fi 名称
const char* password="12345678";  // Wi-Fi 密码

ESP8266WebServer server(80);     // 建立 HTTP 服务器
WebSocketsServer ws(81);         // 建立 WebSocket 服务器
Servo esc, rudder;               // 定义电调与舵机
bool armed=false;                // 电机安全锁状态

// 内嵌网页（摇杆 + Arm 按钮）
String page=R"rawliteral(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<style>
body{margin:0;background:#222;color:#fff;font-family:sans-serif;text-align:center}
#joyL,#joyR{width:120px;height:120px;border-radius:50%;background:#555;position:relative;margin:20px auto;touch-action:none}
.knob{width:60px;height:60px;border-radius:50%;background:#0f0;position:absolute;left:30px;top:30px}
button{padding:10px 20px;font-size:20px;margin:10px}
</style></head><body>
<h2>Boat Control</h2>
<button id='arm'>Arm</button>
<div style="display:flex;justify-content:space-around">
<div><div>Rudder</div><div id="joyL"><div class="knob"></div></div></div>
<div><div>Motor</div><div id="joyR"><div class="knob"></div></div></div>
</div>
<script>
// 建立 WebSocket 连接
let ws=new WebSocket("ws://"+location.hostname+":81/"),armed=false;

// 摇杆控制函数（id=元素ID, cb=回调函数）
function joy(id,cb){
  let j=document.querySelector(id),k=j.firstChild,s=0,t=0;
  j.onpointerdown=e=>{s=e.clientX;t=e.clientY;}; // 记录起点
  j.onpointermove=e=>{
    if(s){
      let dx=e.clientX-s,dy=e.clientY-t;
      let mx=Math.max(-50,Math.min(50,dx)),my=Math.max(-50,Math.min(50,dy));
      k.style.left=30+mx+"px";k.style.top=30+my+"px";
      cb(mx,my); // 回调发送数据
    }};
  j.onpointerup=e=>{ // 松开回中
    s=0;t=0;k.style.left="30px";k.style.top="30px";cb(0,0);
  };
}

// 左摇杆控制舵机（x偏移 → 角度）
joy("#joyL",(x,y)=>{let val=90+x;ws.send("R"+val);});

// 右摇杆控制油门（y偏移 → PWM），需解锁
joy("#joyR",(x,y)=>{
  if(!armed)return; // 未解锁禁止
  let val=1500-y*5; // 中立1500，上推加速
  ws.send("M"+val);
});

// Arm 按钮：切换解锁状态
document.getElementById("arm").onclick=()=>{
  armed=!armed;ws.send("A"+(armed?1:0));
  document.getElementById("arm").style.background=armed?"#f00":"#0f0";
};
</script></body></html>
)rawliteral";

void setup(){
  // 建立 Wi-Fi AP
  WiFi.softAP(ssid,password);

  // 绑定 PWM 引脚（D1 电调，D2 舵机）
  esc.attach(D1,1000,2000); 
  rudder.attach(D2,500,2500);

  // 上电初始化（电机中立，舵机居中）
  esc.writeMicroseconds(1500);
  rudder.write(90);

  // HTTP 服务器返回网页
  server.on("/",[]{server.send(200,"text/html",page);});
  server.begin();

  // WebSocket 事件处理
  ws.begin(); 
  ws.onEvent([](uint8_t n,WStype_t t,uint8_t *p,size_t l){
    if(t==WStype_TEXT){
      String m=(char*)p;
      if(m[0]=='A') armed=m[1]=='1'; // Arm 切换
      if(m[0]=='M' && armed) esc.writeMicroseconds(m.substring(1).toInt()); // 油门
      if(m[0]=='R') rudder.write(m.substring(1).toInt()); // 舵机
    }
  });
}

void loop(){
  server.handleClient(); // 处理 HTTP 请求
  ws.loop();              // 处理 WebSocket
}
