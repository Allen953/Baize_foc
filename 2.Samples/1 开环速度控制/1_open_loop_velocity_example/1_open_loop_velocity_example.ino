// Deng's FOC 开环位置控制例程 测试库：SimpleFOC 2.1.1 测试硬件：灯哥开源FOC V1.0
// 串口中输入"T+数字"设定两个电机的位置，如设置电机转到到180度，输入 "T3.14"（弧度制的180度）
// 在使用自己的电机时，请一定记得修改默认极对数，即 BLDCMotor(7) 中的值，设置为自己的极对数数字
// 程序默认设置的供电电压为 7.4V,用其他电压供电请记得修改 voltage_power_supply , voltage_limit 变量中的值
//               FOC_0.2  Baize_Foc
// IN1     pwm1     9        17
// IN2     pwm2     6        18
// IN3     pwm3     5        19
// INH1   enable1   8        21
// INH2   enable2   7        22
// INH3   enable3   4        23
// 电机极对数:11            减速比:11
//in-line current sense - phase 1/A 35
//in-line current sense - phase 1/C 34
// ESP32  iic接口  SCL:26   SDA:25
// AS5600 iic地址: 0x36
//I2Cone.begin(sda, scl, frequency); 


#include <SimpleFOC.h>

BLDCMotor motor = BLDCMotor(11);
BLDCDriver3PWM driver = BLDCDriver3PWM(17,18,19,21,22,23);

//目标变量
float target_velocity = 5.0;

//串口指令设置
Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&target_velocity, cmd); }

void setup() {
  driver.voltage_power_supply = 12.0;
  driver.init();
  motor.linkDriver(&driver);
  motor.voltage_limit = 12.0;   // [V]
  motor.velocity_limit = 40; // [rad/s]
  
  //开环控制模式设置
  motor.controller = MotionControlType::velocity_openloop;

  //初始化硬件
  motor.init();

  //增加 T 指令
  command.add('T', doTarget, "target velocity");

  Serial.begin(115200);
  Serial.println("Motor ready!");
  Serial.println("Set target velocity [rad/s]");
  _delay(1000);
  
}

void loop() {
  motor.move(target_velocity);

  //用户通讯
  command.run();
}
