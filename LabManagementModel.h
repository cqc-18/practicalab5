#pragma once

using namespace System;
using namespace System;
using namespace System::Collections::Generic;

namespace LabManagementModel {

    // Clase que representa dispositivos genéricos en el laboratorio
    public ref class Device {
    public:
        property String^ Id;
        property bool Status;
        String^ ReportStatus() { 
            return String::Format("Device {0}: {1}", Id, Status ? "Encendido" : "Apagado");
        };
        void TurnOn() {
            Status = true;
        };
        void TurnOff(){
            Status = false;
        };
    };

    // Clase que agrupa sensores
    public ref class Sensor : Device {
    public:
        property float Accuracy;
        property String^ MeasurementRange;
        float MeasureValue() {
            return 0;
        };
        void Calibrate() {};
    };

    // Clase que agrupa actuadores
    public ref class Actuator: Device {
    public:
        property float Power;
        void Activate() {};
        void Stop() {};
    };

    // Representa un robot manipulador que mueve objetos
    public ref class ManipulatorRobot : Device {
    public:
        property array<float>^ Location; // x, y, z coordinates
        property float LoadCapacity;
        property float Speed;

        ManipulatorRobot(String^ id, float capacity, float spd) {
            Id = id;
            Status = false;
            Location = gcnew array<float>(3);
            LoadCapacity = capacity;
            Speed = spd;
        }

        String^ ReportStatus() {
            return String::Format("Robot {0}: {1}", Id, Status ? "Encendido" : "Apagado");
        }

        void MoveArm(array<float>^ coordinates) {
            if (Status) {
                Location = coordinates;
            }
        }

        void RotateJoint(float degrees) {
            if (Status) {
                // Simular rotación de articulación
            }
        }
    };

    // Sensor que mide la temperatura
    public ref class TemperatureSensor : Sensor {
    public:
        property float CurrentValue;

        TemperatureSensor(String^ id, float acc, String^ range) {
            Id = id;
            Status = false;
            Accuracy = acc;
            MeasurementRange = range;
            CurrentValue = 0.0f;
        }

        String^ ReportStatus() {
            return String::Format("SensorTemp {0}: {1}, Valor: {2}°C", Id, Status ? "Activo" : "Inactivo", CurrentValue);
        }

        float MeasureValue() {
            if (Status) {
                CurrentValue = 25.0f; // Medición simulada
            }
            return CurrentValue;
        }

        void Calibrate() {
            if (Status) {
                CurrentValue = 0.0f; // Reiniciar para calibración
            }
        }

        float ReportTemperature() {
            return MeasureValue();
        }
    };

    // Sensor que detecta proximidad
    public ref class ProximitySensor : Sensor {
    public:
        property float CurrentDistance;

        ProximitySensor(String^ id, float acc, String^ range) {
            Id = id;
            Status = false;
            Accuracy = acc;
            MeasurementRange = range;
            CurrentDistance = 0.0f;
        }

        String^ ReportStatus() {
            return String::Format("SensorProx {0}: {1}, Distancia: {2}cm", Id, Status ? "Activo" : "Inactivo", CurrentDistance);
        }

        float MeasureValue() {
            if (Status) {
                CurrentDistance = 10.0f; // Medición simulada
            }
            return CurrentDistance;
        }

        void Calibrate() {
            if (Status) {
                CurrentDistance = 0.0f; // Reiniciar para calibración
            }
        }

        bool DetectObject() {
            return Status && CurrentDistance < 50.0f;
        }
    };

    // Motor eléctrico que proporciona movimiento
    public ref class ElectricMotor : Actuator {
    public:
        property float Speed;

        ElectricMotor(String^ id, float pwr, float spd) {
            Id = id;
            Status = false;
            Power = pwr;
            Speed = spd;
        }

        String^ ReportStatus() {
            return String::Format("Motor {0}: {1}, Velocidad: {2}RPM", Id, Status ? "Activo" : "Inactivo", Speed);
        }

        void Activate() {
            Status = true;
        }

        void Stop() {
            Status = false;
            Speed = 0.0f;
        }

        void AdjustSpeed(float RPM) {
            if (Status) {
                Speed = RPM;
            }
        }
    };

    // Válvula neumática que regula presión
    public ref class PneumaticValve : Actuator {
    public:
        property float Pressure;

        PneumaticValve(String^ id, float pwr, float pres) {
            Id = id;
            Status = false;
            Power = pwr;
            Pressure = pres;
        }

        String^ ReportStatus() {
            return String::Format("Valvula {0}: {1}, Presion: {2}bar", Id, Status ? "Activa" : "Inactiva", Pressure);
        }

        void Activate() {
            Status = true;
        }

        void Stop() {
            Status = false;
        }

        void Open() {
            if (Status) {
                Pressure = 1.0f; // Simular estado abierto
            }
        }

        void Close() {
            if (Status) {
                Pressure = 0.0f; // Simular estado cerrado
            }
        }

        void RegulatePressure(float bar) {
            if (Status) {
                Pressure = bar;
            }
        }
    };

    // Estación que coordina dispositivos
    public ref class ControlStation {
    public:
        property String^ StationId;
        property bool Status;
        property List<Device^>^ ConnectedDevices;

        ControlStation(String^ id) {
            StationId = id;
            Status = false;
            ConnectedDevices = gcnew List<Device^>();
        }

        void StartExperiment() {
            Status = true;
            for each (Device ^ d in ConnectedDevices) {
                d->TurnOn();
            }
        }

        void StopExperiment() {
            Status = false;
            for each (Device ^ d in ConnectedDevices) {
                d->TurnOff();
            }
        }

        void AssignTask(Device^ device) {
            if (Status && !ConnectedDevices->Contains(device)) {
                ConnectedDevices->Add(device);
            }
        }

        List<String^>^ MonitorDevices() {
            List<String^>^ reports = gcnew List<String^>();
            for each (Device ^ d in ConnectedDevices) {
                reports->Add(d->ReportStatus());
            }
            return reports;
        }
    };

    // Sistema que registra datos
    public ref class DataLoggingSystem {
    public:
        property String^ LogId;
        property DateTime Date;
        property DateTime Time;
        property String^ DataType;
        property float RecordedValue;

        DataLoggingSystem(String^ id, String^ type, float value) {
            LogId = id;
            Date = DateTime::Now;
            Time = DateTime::Now;
            DataType = type;
            RecordedValue = value;
        }

        void SaveData(Object^ data) {
            RecordedValue = Convert::ToSingle(data);
            Date = DateTime::Now;
            Time = DateTime::Now;
        }

        List<Object^>^ QueryLogs(DateTime queryDate) {
            List<Object^>^ logs = gcnew List<Object^>();
            if (queryDate.Date == Date.Date) {
                logs->Add(RecordedValue);
            }
            return logs;
        }

        String^ GenerateReport() {
            return String::Format("Registro {0}: {1} = {2} ({3})", LogId, DataType, RecordedValue, Date.ToString());
        }
    };

    // Representa un experimento
    public ref class Experiment {
    public:
        property String^ ExperimentId;
        property DateTime StartDate;
        property int Duration;
        property String^ Status;

        Experiment(String^ id, int dur) {
            ExperimentId = id;
            StartDate = DateTime::Now;
            Duration = dur;
            Status = "En curso";
        }

        void Start() {
            Status = "En curso";
            StartDate = DateTime::Now;
        }

        void End() {
            Status = "Finalizado";
        }

        String^ GenerateReport() {
            return String::Format("Experimento {0}: {1}, Inicio: {2}", ExperimentId, Status, StartDate.ToString());
        }
    };

    // Gestiona asignación de dispositivos a experimentos
    public ref class ExperimentAssignment {
    public:
        property String^ AssignmentId;
        property DateTime AssignmentDate;
        property String^ DeviceRole;
        property Device^ TheDevice;
        property Experiment^ TheExperiment;

        ExperimentAssignment(String^ id, Device^ dev, Experiment^ exp, String^ role) {
            AssignmentId = id;
            AssignmentDate = DateTime::Now;
            DeviceRole = role;
            TheDevice = dev;
            TheExperiment = exp;
        }
    };
}
