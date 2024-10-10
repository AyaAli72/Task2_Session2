#ifndef TorpedoMPU_H_
#define TorpedoMPU_H_
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

class TorpedoMPU: public MPU6050 {
  private:
    Quaternion myQuatOffset;
    bool dmpReady = false;
    uint8_t mpuIntStatus;
    uint8_t devStatus;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];
    Quaternion q;
    VectorFloat gravity;
    float ypr[3];
    volatile bool mpuInterrupt = false;
    boolean stable_1 = false;
    float myYawOffset = 0.0f;
    boolean stable_2 = false;
    long Time = 0;
    float sinYaw = 0;
    float cosYaw = 0;
    float sinPitch = 0;
    float cosPitch = 0;
    float sinRoll = 0;
    float cosRoll = 0;
    float x2 = 0;
    float z2 = 0;
    float  accx, accy, accz;
    float gyrx, gyry, gyrz;
    float roll, pitch, yaw_3;
  public:
    int angles[3];
  private:
    static TorpedoMPU::QuaternionChange(float x, float y, float z)
    {
      return new Quaternion(x, y , z, 1);
    }

  public:
    TorpedoMPU() {
      this->roll = 0;
      this->pitch = 0;
      this->yaw_3 = 0;
    };
    void TorpedoMPU::start() {
      mpuInterrupt = true;
      Wire.begin();
      Wire.setClock(400000);
      while (!Serial);
      Serial.println(F("Initializing I2C devices..."));
      initialize();
      Serial.println(F("Testing device connections..."));
      Serial.println(testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

      Serial.println(F("Initializing DMP..."));
      devStatus = dmpInitialize();

      setXGyroOffset(220);
      setYGyroOffset(76);
      setZGyroOffset(-85);
      setZAccelOffset(1788);
    }
    void TorpedoMPU::check() {
      if (devStatus == 0)
      {
        CalibrateAccel(6);
        CalibrateGyro(6);
        PrintActiveOffsets();
        Serial.println(F("Enabling DMP..."));
        setDMPEnabled(true);

        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        mpuIntStatus = getIntStatus();
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        packetSize = dmpGetFIFOPacketSize();

      }
      else
      {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
      }
    }
    void TorpedoMPU:: SetOffset(Quaternion *offset, Quaternion *q) {
      offset->w = q->w;
      offset->x = q->x;
      offset->y = q->y;
      offset->z = q->z;
    }
    float TorpedoMPU:: getYaw_mine(Quaternion &q)
    {
      x2 = q.x * q.x;
      z2 = q.z * q.z;
      return  atan2(2.0f * q.z * q.w - 2.0f * q.y * q.x, 1.0f - 2.0f * z2 - 2.0f * x2);
    }
    void TorpedoMPU:: createFromYawPitchRoll_mine(float yaw_3, float pitch, float roll) {

      yaw_3 = 0.5f;
      pitch *= 0.5f;
      roll *= 0.5f;
      sinYaw = sin(yaw_3);
      cosYaw = cos(yaw_3);
      sinPitch = sin(pitch);
      cosPitch = cos(pitch);
      sinRoll = sin(roll);
      cosRoll = cos(roll);
      QuaternionChange(q.x, q.y, q.z);

#ifdef SWITCH_PITCH_AND_ROLL_AXIS
#ifdef ROTATE_180_DEGREE_Z_YAW
      q.w =   cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
      q.x = -(sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw);
      q.y = -(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw);
      q.z = -(sinRoll * sinPitch * cosYaw - cosRoll * cosPitch * sinYaw);
#else
      q.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
      q.x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
      q.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
      q.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
#endif
#else
#ifdef ROTATE_180_DEGREE_Z_YAW
      q.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
      q.x = -(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw);
      q.y = -(cosRoll * sinPitch * sinYaw - sinRoll * cosPitch * cosYaw);
      q.z = -(sinRoll * sinPitch * cosYaw - cosRoll * cosPitch * sinYaw);
#else
      q.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
      q.x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
      q.y = cosRoll * sinPitch * sinYaw - sinRoll * cosPitch * cosYaw;
      q.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
#endif
#endif
      return q;
    }
    void TorpedoMPU::calculate() {
      if (!dmpReady) return;
      mpuInterrupt = false;
      mpuIntStatus = getIntStatus();
      fifoCount = getFIFOCount();
      if ((mpuIntStatus & 0x10) || fifoCount == 1024)
      {
        resetFIFO();
        Serial.println(F("FIFO overflow!"));
      }
      else if (mpuIntStatus & 0x02)
      {
        while (fifoCount < packetSize) fifoCount = getFIFOCount();
        getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        dmpGetQuaternion(&q, fifoBuffer);
        dmpGetGravity(&gravity, &q);
        dmpGetYawPitchRoll(ypr, &q, &gravity);
        if (!stable_1) {
          Time = millis();
          if ((int)(2.0f * ypr[0] * 180 / M_PI) == (int)(myYawOffset)) {
            stable_2 = true;
          }
          if (stable_2) {
            if (Time + 1000 < millis()) stable_1 = true;
          } else {
            Time = millis();
          }
          myYawOffset = 2.0f * ypr[0] * 180 / M_PI;
          SetOffset(&myQuatOffset, &q);
          if (stable_1) {
            float yaw_1 = getYaw_mine(q);
            float yaw_2 = getYaw_mine(myQuatOffset);
            float yaw_3 = yaw_1 - yaw_2;
            createFromYawPitchRoll_mine( yaw_3, pitch, roll);
          }

        }
        if (abs(yaw_3 * 180 / M_PI - ypr[0] * 180 / M_PI) > 0.5) {
          yaw_3 = ypr[0];
        }
        roll = (ypr[2] * 180 / M_PI);
        pitch = (ypr[1] * 180 / M_PI);
        yaw_3 = (yaw_3 * 180 / M_PI) ;
      }
      angles[0]=roll;
      angles[1]=pitch;
      angles[2]=yaw_3;
    }
//     float TorpedoMPU::Return_roll() {
//       calculate();
//       return roll;
//     }
//     float TorpedoMPU::Return_pitch() {
//       calculate();
//       return pitch;
//     }
//     float  TorpedoMPU::Return_yaw() {
//       calculate();
//       return yaw_3;
//     }
    //    int TorpedoMPU::Return_accx() {
    //      accx = getAccelerationX();
    //      return accx;
    //    }
    //    int TorpedoMPU::Return_accy() {
    //      accy = getAccelerationY();
    //      return accy;
    //    }
    //    int  TorpedoMPU::Return_accz() {
    //      accz = getAccelerationZ();
    //      return accz;
    //    }
    //    int  TorpedoMPU::Return_gyrx() {
    //      gyrx = getRotationX();
    //      return gyrx;
    //    }
    //    int TorpedoMPU::Return_gyry() {
    //      gyry = getRotationY();
    //      return gyry;
    //    }
    //    int TorpedoMPU::Return_gyrz() {
    //      gyrx = getRotationZ();
    //      return gyrz;
    //    }
    int *TorpedoMPU:: communication_data() {
      calculate();
      return angles;
    }
};
#endif
