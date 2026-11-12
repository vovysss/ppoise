#include <iostream>
#include "hardware/Component.h"
#include "hardware/Core.h"
#include "hardware/Processor.h"
#include "hardware/RAMModule.h"
#include "hardware/PowerSupplyUnit.h"
#include "hardware/Motherboard.h"
#include "hardware/HDD.h"
#include "hardware/SSD.h"
#include "hardware/VRAM.h"
#include "hardware/GPUCore.h"
#include "hardware/GraphicsCard.h"
#include "hardware/Fan.h"
#include "hardware/CoolingSystem.h"
#include "hardware/CacheMemory.h"
#include "hardware/ROMModule.h"
#include "hardware/ComputerCase.h"
#include "hardware/Chipset.h"
#include "hardware/Socket.h"
#include "hardware/Monitor.h"
#include "hardware/Keyboard.h"
#include "hardware/Mouse.h"
#include "hardware/Printer.h"
#include "hardware/Scanner.h"
#include "hardware/Webcam.h"
#include "hardware/Microphone.h"
#include "hardware/Speaker.h"
#include "hardware/USBController.h"
#include "hardware/SoundCard.h"
#include "hardware/WiFiAdapter.h"
#include "hardware/BluetoothAdapter.h"
#include "hardware/Battery.h"
#include "hardware/TemperatureSensor.h"
#include "hardware/Touchpad.h"
#include "network/Router.h"
#include "network/FirewallRule.h"
#include "software/File.h"
#include "software/Driver.h"
#include "software/Process.h"
#include "software/OperatingSystem.h"
#include "software/UserAccount.h"
#include "software/UserSession.h"
#include "software/FileSystem.h"
#include "software/Directory.h"
#include "software/Kernel.h"
#include "software/Scheduler.h"
#include "software/MemoryManager.h"
#include "software/DeviceManager.h"
#include "software/Antivirus.h"
#include "software/BackupService.h"
#include "software/UpdateService.h"
#include "software/Compiler.h"
#include "software/VirtualMachine.h"
#include "exceptions/OverheatingException.h"
#include "exceptions/AuthenticationException.h"

int main() {
    std::cout << "=== Полная проверка аппаратной подсистемы (Hardware Suite) ===\n\n";

    try {
        // 1. Создаем корпус и блок питания
        hardware::ComputerCase pcCase("CASE-01", "NZXT H5 Flow", 5.0, 28.0, "Mid-Tower", 365.0);
        hardware::PowerSupplyUnit psu("PSU-01", "BeQuiet Dark Power 1000W", 15.0, 32.0, 1000.0, "80+ Platinum");

        // 2. Создаем постоянную память (BIOS/UEFI) и материнскую плату
        hardware::ROMModule bios("ROM-01", "AMI UEFI BIOS", 2.0, 26.0, 32768, "v4.20");
        hardware::Motherboard mb("MB-01", "MSI MPG Z790 Carbon", 25.0, 35.0, "Intel Z790", 1700, 4);

        // 3. Создаем процессор и модуль оперативной памяти
        hardware::Processor cpu("CPU-01", "Intel Core i7-14700K", 125.0, 40.0, 3.4, 1700, 8);
        hardware::RAMModule ram1("RAM-01", "Corsair Vengeance", 8.0, 33.0, 32, 6000.0, "DDR5");

        // 4. Создаем накопители (HDD и SSD)
        hardware::HDD hdd("HDD-01", "Seagate Barracuda 2TB", 12.0, 30.0, 2048, 190.0, 185.0, 7200, 24.5);
        hardware::SSD ssd("SSD-01", "Samsung 990 PRO 2TB", 7.0, 38.0, 2048, 7450.0, 6900.0, "TLC", 1200);

        // 5. Создаем видеоподсистему (VRAM + видеокарта с ядрами GPU)
        hardware::VRAM vram("VRAM-01", "GDDR6X Video Buffer", 35.0, 42.0, 16, 960.0, "GDDR6X");
        hardware::GraphicsCard gpu("GPU-01", "NVIDIA RTX 4080 Super", 320.0, 45.0, "AD103", 2550.0, 3, &vram);

        // 6. Создаем систему охлаждения с вентиляторами
        hardware::CoolingSystem cooler("COOL-01", "DeepCool Liquid Cooler", 12.0, 28.0, "Liquid", 0.9, 3, 2000.0, 120.0);

        // --- Этап включения и сборки ---
        std::cout << "--- [1] Включение базовой инфраструктуры ---\n";
        pcCase.powerOn();
        psu.powerOn();
        bios.powerOn();
        mb.powerOn();
        cooler.powerOn();

        std::cout << "\n--- [2] Установка комплектующих в материнскую плату ---\n";
        mb.installProcessor(&cpu, 1700);
        mb.installRAM(&ram1);

        std::cout << "\n--- [3] Запуск накопителей и графики ---\n";
        hdd.powerOn();
        ssd.powerOn();
        gpu.powerOn();

        std::cout << "\n--- [4] Тестирование операций чтения/записи и рендеринга ---\n";
        ssd.readData(1024);
        hdd.writeData(512);
        gpu.renderFrame(75.0);

        std::cout << "\n--- [5] Проверка работы охлаждения ---\n";
        double safeTemp = cooler.dissipateHeat(cpu.getTemperature(), 20.0);
        std::cout << "Текущая стабильная температура процессора: " << safeTemp << "°C\n";

        std::cout << "\n--- [6] Подключение периферийных устройств ---\n";
        hardware::Monitor monitor("MON-01", "Dell UltraSharp 27", 35.0, 30.0,
                                  27.0, 3840, 2160, 144, "IPS", 400, &gpu);
        hardware::Keyboard keyboard("KBD-01", "Logitech MX Keys", 2.0, 26.0, "ISO", 105);
        hardware::Mouse mouse("MSE-01", "Logitech MX Master", 1.5, 26.0, 8000, 7, true);
        hardware::Printer printer("PRN-01", "HP LaserJet Pro", 18.0, 29.0, 80, 50);
        hardware::Scanner scanner("SCN-01", "Epson Perfection", 12.0, 28.0, 1200, 24);
        hardware::Webcam webcam("CAM-01", "Logitech Brio", 3.0, 27.0, 4.0, 60);
        hardware::Microphone microphone("MIC-01", "Blue Yeti", 2.0, 26.0, -20.0, 48000);
        hardware::Speaker speakers("SPK-01", "Edifier R1280", 20.0, 27.0, 2, 20000.0);
        hardware::USBController usb("USB-01", "Intel USB 3.2", 5.0, 30.0, 4, "USB 3.2 Gen2", 7.5);
        hardware::SoundCard soundCard("SND-01", "Creative Sound Blaster", 8.0, 31.0, 6, 192, 24);
        hardware::WiFiAdapter wifi("WIFI-01", "Intel Wi-Fi 6E", 4.0, 32.0, "6 GHz");
        hardware::BluetoothAdapter bluetooth("BT-01", "Intel Bluetooth 5.3", 1.0, 27.0, "5.3");
        hardware::Battery battery("BAT-01", "UPS Back-UPS 1500", 0.0, 25.0, 9000, &pcCase);
        hardware::TemperatureSensor temperatureSensor("TMP-01", "CPU Temp Sensor", 1.0, 30.0,
                                                      75.0, 90.0, &cpu);
        hardware::Touchpad touchpad("TPD-01", "Precision Touchpad", 1.0, 26.0, 120, 70);
        software::File report("lab2-report", 12, "pdf");
        software::File sourceFile("main", 2, "cpp");
        software::File videoFile("movie", 800, "mp4");

        monitor.powerOn();
        monitor.displayFrame("3D-сцена лабораторной работы");
        monitor.adjustBrightness(50);
        monitor.calibrateColors();
        keyboard.powerOn();
        keyboard.toggleBacklight();
        keyboard.typeText("OpenCode");
        mouse.powerOn();
        mouse.moveCursor(120, 80);
        mouse.clickButton(1);
        printer.powerOn();
        printer.printDocument(report);
        scanner.powerOn();
        scanner.scanPage(report);
        webcam.powerOn();
        webcam.startStream();
        webcam.capturePhoto();
        microphone.powerOn();
        microphone.captureAudio(15);
        speakers.powerOn();
        speakers.playSound("system-notification");
        soundCard.powerOn();
        soundCard.synthesizeSound(440);
        soundCard.routeAudio("Speakers");
        usb.powerOn();
        usb.connectDevice(&keyboard);
        usb.connectDevice(&mouse);
        usb.checkPowerBudget(20.0);
        bluetooth.powerOn();
        bluetooth.pairDevice("Wireless Headset");
        bluetooth.sendFile(sourceFile);
        battery.powerOn();
        battery.reportHealth();
        temperatureSensor.powerOn();
        temperatureSensor.triggerAlarm();
        touchpad.powerOn();
        touchpad.detectGesture("two-finger-scroll");
        touchpad.scroll(-5);

        std::cout << "\n--- [7] Программная подсистема и безопасность ---\n";
        software::UserAccount user("student", "secret123", "admin");
        software::UserSession session("SESSION-01", "2026-11-12 12:00", &user);
        user.authenticate("secret123");
        session.startSession();
        session.logActivity("запуск лабораторной работы");

        software::Directory documents("documents", "/home/student/documents");
        documents.addFile(sourceFile);
        documents.addFile(report);
        documents.listContents();
        documents.findFile("main");

        software::FileSystem fileSystem("/home/student", 512.0);
        fileSystem.createFile(sourceFile);
        fileSystem.createFile(report);
        fileSystem.checkIntegrity();

        software::Kernel kernel("6.8.0");
        kernel.boot();
        software::Driver gpuDriver("NVIDIA Driver", "GPU", "555.42");
        kernel.loadModule(gpuDriver);

        software::Process compilerProcess(101, "g++", 512, 35.0);
        kernel.scheduleProcess(compilerProcess);
        software::Scheduler scheduler("RoundRobin", 10);
        scheduler.enqueue(compilerProcess);
        scheduler.scheduleNext();
        scheduler.calculatePriority(compilerProcess);

        software::MemoryManager memoryManager(16, 4, &ram1);
        memoryManager.allocatePages(2048);
        memoryManager.checkFragmentation();

        software::DeviceManager deviceManager;
        deviceManager.registerDevice(&keyboard);
        deviceManager.registerDevice(&mouse);
        deviceManager.scanDevices();

        software::Antivirus antivirus("2026.11.01");
        antivirus.activate();
        antivirus.scanFile(videoFile);
        antivirus.quarantine(videoFile.getName());

        software::BackupService backupService("/backup/student", &fileSystem);
        backupService.createBackup();
        backupService.verifyBackup();

        software::UpdateService updateService("1.0.0");
        updateService.checkForUpdates("1.1.0");
        updateService.installUpdate();

        software::Compiler compiler("C++", 2);
        compiler.compile(sourceFile);
        compiler.reportErrors();

        software::OperatingSystem guestOS("Linux Guest", "6.8");
        software::VirtualMachine virtualMachine("VM-01", 8, 4, &guestOS);
        virtualMachine.startVM();
        virtualMachine.snapshot();

        std::cout << "\n--- [8] Сеть и firewall ---\n";
        hardware::WiFiAdapter wifiAdapter("WIFI-02", "Intel AX211", 4.0, 32.0, "5 GHz");
        network::NetworkPacket packet(42, "10.0.0.5", 128);
        network::NetworkPacket secondPacket(43, "10.0.0.5", 64);
        network::FirewallRule allowRule("allow-lab", "192.168.0.10", "10.0.0.5", 443, true);
        network::Router router("192.168.0.1", true);
        router.start();
        router.addFirewallRule(allowRule);
        router.assignIP("student-laptop");
        router.forwardPacket(packet);
        wifiAdapter.powerOn();
        wifiAdapter.scanNetworks();
        wifiAdapter.connectToNetwork("HomeNet");
        wifiAdapter.sendPacket(secondPacket);

        std::cout << "\n--- [9] Проверка собственных исключений ---\n";
        software::UserAccount intruder("intruder", "correct-password", "guest");
        try {
            intruder.authenticate("wrong-1");
            intruder.authenticate("wrong-2");
            intruder.authenticate("wrong-3");
        } catch (const exceptions::AuthenticationException& e) {
            std::cerr << "[ПЕРЕХВАТ АУТЕНТИФИКАЦИИ]: " << e.what() << "\n";
        }

        std::cout << "\n--- [10] Тест экстремального разгона (проверка OverheatingException) ---\n";
        // Повторный разгон поднимает температуру выше критической отметки 85°C
        cpu.overclock(95.0);
        cpu.overclock(95.0);

    } catch (const exceptions::OverheatingException& e) {
        std::cerr << "\n[ПЕРЕХВАТ АВАРИИ]: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "\n[ОБЩАЯ ОШИБКА]: " << e.what() << "\n";
    }

    std::cout << "\nСимуляция аппаратной части завершена успешно.\n";
    return 0;
}