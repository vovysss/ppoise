# Лабораторная работа №2. Объектная модель предметной области «Компьютер»

## 1. О проекте

Учебный C++-проект моделирует компьютер как совокупность аппаратной, сетевой и программной подсистем. Модель демонстрирует наследование, полиморфизм, композицию, агрегацию, ассоциации и собственные исключения.

Домен: **персональный компьютер и его окружение** — комплектующие, периферия, операционная система, файловая система, процессы, пользователи, сеть и средства диагностики.

### Сборка и запуск

```bash
g++ -std=c++17 -I. main.cpp -o pc_sim
./pc_sim
```

### Правила подсчёта

- **Поля** — собственные поля класса; унаследованные поля не дублируются.
- **Методы** — содержательные поведения предметной области. Геттеры, сеттеры, конструкторы и деструкторы не учитываются.
- **Ассоциации** — случаи, когда класс хранит другой доменный класс в поле или принимает его в методе.
- Исключения учитываются отдельно от доменных классов.

## 2. Итоговая статистика

| Показатель | Значение 
|---|---:|---:|
| Доменные классы | 57 
| Поля | 218 
| Поведения (методы) | 207 
| Ассоциации | 33 
| Собственные исключения | 15 

## 3. Сводная таблица классов

Формат: класс | число полей | число поведений | связанные классы.

| Класс | Поля | Методы | Ассоциации (связанные классы) |
|---|---:|---:|---|
| Component | 5 | 1 | — |
| AbstractProcessor | 3 | 2 | — |
| Processor | 3 | 4 | Core |
| Core | 3 | 1 | — |
| CacheMemory | 4 | 3 | — |
| Chipset | 4 | 2 | — |
| ComputerCase | 3 | 3 | — |
| CoolingSystem | 3 | 3 | Fan |
| Fan | 3 | 3 | — |
| GPUCore | 4 | 2 | — |
| GraphicsCard | 4 | 3 | GPUCore, VRAM |
| HDD | 2 | 3 | — |
| MemoryModule | 2 | 1 | — |
| Motherboard | 6 | 5 | Processor, RAMModule |
| PowerSupplyUnit | 2 | 3 | — |
| RAMModule | 3 | 3 | — |
| ROMModule | 2 | 3 | — |
| SSD | 2 | 3 | — |
| Socket | 3 | 2 | — |
| StorageDevice | 4 | 2 | — |
| VRAM | 3 | 2 | — |
| Monitor | 8 | 6 | GraphicsCard |
| Keyboard | 5 | 5 | — |
| Mouse | 6 | 5 | — |
| Printer | 4 | 5 | File |
| Scanner | 4 | 4 | File |
| Webcam | 4 | 5 | — |
| Microphone | 4 | 5 | — |
| Speaker | 4 | 6 | — |
| USBController | 4 | 5 | Component |
| SoundCard | 4 | 4 | — |
| WiFiAdapter | 4 | 5 | NetworkPacket |
| BluetoothAdapter | 3 | 5 | File |
| Battery | 5 | 5 | Component |
| TemperatureSensor | 4 | 3 | Component |
| Touchpad | 4 | 5 | — |
| NetworkCard | 3 | 3 | NetworkPacket |
| NetworkPacket | 4 | 1 | — |
| Router | 5 | 6 | FirewallRule, NetworkPacket |
| FirewallRule | 5 | 4 | NetworkPacket |
| Driver | 4 | 2 | — |
| File | 3 | 1 | — |
| OperatingSystem | 3 | 3 | Process |
| Process | 5 | 2 | — |
| UserAccount | 5 | 5 | — |
| UserSession | 4 | 4 | UserAccount |
| FileSystem | 5 | 4 | Directory, File |
| Directory | 4 | 4 | File, Directory |
| Kernel | 3 | 5 | Driver, Process |
| Scheduler | 4 | 5 | Process |
| MemoryManager | 4 | 5 | RAMModule |
| DeviceManager | 3 | 4 | Component |
| Antivirus | 3 | 5 | File |
| BackupService | 4 | 4 | FileSystem |
| UpdateService | 3 | 4 | — |
| Compiler | 4 | 4 | File |
| VirtualMachine | 5 | 5 | OperatingSystem |

## 4. Подробное описание классов

### Аппаратная подсистема

**Component**

- Поля (5): id, name, powerConsumption, temperature, isWorking.
- Методы (1): checkHealth.
- Ассоциации: нет.

**AbstractProcessor**

- Поля (3): coreCount, clockFrequencyGHz, socketType.
- Методы (2): executeInstructions, overclock.
- Ассоциации: нет.

**Processor**

- Поля (3): baseFrequency, socketType, cores.
- Методы (4): powerOn, powerOff, overclock, distributeLoad.
- Ассоциации: Core.

**Core**

- Поля (3): coreId, currentLoad, frequency.
- Методы (1): executeInstruction.
- Ассоциации: нет.

**CacheMemory**

- Поля (4): cacheLevel, sizeKB, latencyNs, isActive.
- Методы (3): activate, deactivate, fetchCache.
- Ассоциации: нет.

**Chipset**

- Поля (4): chipsetModel, pcieLanesCount, supportsOverclocking, isActive.
- Методы (2): initialize, shutdown.
- Ассоциации: нет.

**ComputerCase**

- Поля (3): formFactor, maxGPU_LengthMM, isSidePanelOpen.
- Методы (3): powerOn, powerOff, toggleSidePanel.
- Ассоциации: нет.

**CoolingSystem**

- Поля (3): coolingType, fans, coolingEfficiency.
- Методы (3): powerOn, powerOff, dissipateHeat.
- Ассоциации: Fan.

**Fan**

- Поля (3): maxRPM, currentRPM, diameterMM.
- Методы (3): powerOn, powerOff, setSpeed.
- Ассоциации: нет.

**GPUCore**

- Поля (4): coreId, frequencyMHz, currentLoadPercent, isActive.
- Методы (2): activate, deactivate.
- Ассоциации: нет.

**GraphicsCard**

- Поля (4): chipsetModel, gpuCores, videoMemory, coreClockMHz.
- Методы (3): powerOn, powerOff, renderFrame.
- Ассоциации: GPUCore, VRAM.

**HDD**

- Поля (2): rpm, noiseLevelDb.
- Методы (3): powerOn, powerOff, defragment.
- Ассоциации: нет.

**MemoryModule**

- Поля (2): capacityGB, frequencyMHz.
- Методы (1): selfTest.
- Ассоциации: нет.

**Motherboard**

- Поля (6): chipset, socketType, installedProcessor, maxRamSlots, usedRamSlots, ramSlots.
- Методы (5): powerOn, powerOff, installProcessor, installRAM, checkSystemHealth.
- Ассоциации: Processor, RAMModule.

**PowerSupplyUnit**

- Поля (2): maxWattage, efficiencyRating.
- Методы (3): powerOn, powerOff, providePower.
- Ассоциации: нет.

**RAMModule**

- Поля (3): capacityGB, speedMHz, memoryType.
- Методы (3): powerOn, powerOff, allocateMemory.
- Ассоциации: нет.

**ROMModule**

- Поля (2): capacityKB, firmwareVersion.
- Методы (3): powerOn, powerOff, flashFirmware.
- Ассоциации: нет.

**SSD**

- Поля (2): nandType, TBW.
- Методы (3): powerOn, powerOff, trim.
- Ассоциации: нет.

**Socket**

- Поля (3): socketName, pinCount, isOccupied.
- Методы (2): lockSocket, unlockSocket.
- Ассоциации: нет.

**StorageDevice**

- Поля (4): capacityGB, readSpeedMBs, writeSpeedMBs, usedSpaceGB.
- Методы (2): readData, writeData.
- Ассоциации: нет.

**VRAM**

- Поля (3): capacityGB, bandwidthGBs, memoryType.
- Методы (2): powerOn, powerOff.
- Ассоциации: нет.

**Monitor**

- Поля (8): screenSizeInch, resolutionWidth, resolutionHeight, refreshRateHz, panelType, brightnessNits, currentInput, connectedGPU.
- Методы (6): powerOn, powerOff, adjustBrightness, switchInput, calibrateColors, displayFrame.
- Ассоциации: GraphicsCard.

**Keyboard**

- Поля (5): layout, keyCount, isBacklit, backlightLevel, lastPressedKey.
- Методы (5): powerOn, powerOff, pressKey, toggleBacklight, typeText.
- Ассоциации: нет.

**Mouse**

- Поля (6): dpi, buttonCount, isWireless, batteryPercent, cursorX, cursorY.
- Методы (5): powerOn, powerOff, moveCursor, clickButton, changeDpi.
- Ассоциации: нет.

**Printer**

- Поля (4): inkLevelPercent, paperCount, duplexEnabled, printedPageCount.
- Методы (5): powerOn, powerOff, printDocument, refillInk, loadPaper.
- Ассоциации: File.

**Scanner**

- Поля (4): resolutionDpi, colorDepthBits, scannedPages, lampWarmedUp.
- Методы (4): powerOn, powerOff, scanPage, calibrateLamp.
- Ассоциации: File.

**Webcam**

- Поля (4): resolutionMP, fps, isStreaming, capturedPhotos.
- Методы (5): powerOn, powerOff, startStream, stopStream, capturePhoto.
- Ассоциации: нет.

**Microphone**

- Поля (4): sensitivityDb, isMuted, sampleRateHz, recordedSeconds.
- Методы (5): powerOn, powerOff, captureAudio, mute, unmute.
- Ассоциации: нет.

**Speaker**

- Поля (4): channelCount, volumePercent, isMuted, frequencyRangeHz.
- Методы (6): powerOn, powerOff, playSound, increaseVolume, decreaseVolume, mute.
- Ассоциации: нет.

**USBController**

- Поля (4): portCount, usbStandard, powerPerPortWatts, connectedDevices.
- Методы (5): powerOn, powerOff, connectDevice, disconnectDevice, checkPowerBudget.
- Ассоциации: Component.

**SoundCard**

- Поля (4): channels, sampleRateKHz, bitDepth, activeOutput.
- Методы (4): powerOn, powerOff, synthesizeSound, routeAudio.
- Ассоциации: нет.

**WiFiAdapter**

- Поля (4): ssid, signalStrengthDbm, band, isConnected.
- Методы (5): powerOn, powerOff, connectToNetwork, scanNetworks, sendPacket.
- Ассоциации: NetworkPacket.

**BluetoothAdapter**

- Поля (3): bluetoothVersion, pairedDeviceCount, isEnabled.
- Методы (5): powerOn, powerOff, pairDevice, sendFile, disconnectDevice.
- Ассоциации: File.

**Battery**

- Поля (5): capacityMah, chargePercent, cycleCount, isCharging, poweredDevice.
- Методы (5): powerOn, powerOff, charge, discharge, reportHealth.
- Ассоциации: Component.

**TemperatureSensor**

- Поля (4): currentTemp, warningThreshold, criticalThreshold, monitoredComponent.
- Методы (3): powerOn, powerOff, triggerAlarm.
- Ассоциации: Component.

**Touchpad**

- Поля (4): widthMM, heightMM, gestureEnabled, lastGesture.
- Методы (5): powerOn, powerOff, detectGesture, scroll, tap.
- Ассоциации: нет.

### Сетевая подсистема

**NetworkCard**

- Поля (3): macAddress, maxSpeedMbps, isConnected.
- Методы (3): powerOn, powerOff, sendPacket.
- Ассоциации: NetworkPacket.

**NetworkPacket**

- Поля (4): packetId, destinationIP, payloadSizeKB, isDelivered.
- Методы (1): transmit.
- Ассоциации: нет.

**Router**

- Поля (5): ipAddress, dhcpEnabled, connectedClientCount, firewallRules, packetBuffer.
- Методы (6): start, stop, assignIP, forwardPacket, addFirewallRule, restart.
- Ассоциации: FirewallRule, NetworkPacket.

**FirewallRule**

- Поля (5): ruleName, sourceIP, destinationIP, port, isAllowed.
- Методы (4): permits, apply, revoke, describe.
- Ассоциации: NetworkPacket.

### Программная подсистема

**Driver**

- Поля (4): driverName, deviceTarget, version, isLoaded.
- Методы (2): loadDriver, unloadDriver.
- Ассоциации: нет.

**File**

- Поля (3): fileName, fileSizeMB, extension.
- Методы (1): displayInfo.
- Ассоциации: нет.

**OperatingSystem**

- Поля (3): name, version, activeProcesses.
- Методы (3): boot, startProcess, showTaskManger.
- Ассоциации: Process.

**Process**

- Поля (5): processId, name, memoryRequiredMB, cpuLoadPercent, isRunning.
- Методы (2): start, terminate.
- Ассоциации: нет.

**UserAccount**

- Поля (5): username, passwordHash, role, isLocked, failedAttempts.
- Методы (5): authenticate, changePassword, lockAccount, unlockAccount, resetFailedAttempts.
- Ассоциации: нет.

**UserSession**

- Поля (4): sessionId, startTime, isActive, account.
- Методы (4): startSession, endSession, validate, logActivity.
- Ассоциации: UserAccount.

**FileSystem**

- Поля (5): rootPath, totalSpaceGB, usedSpaceGB, directories, files.
- Методы (4): createFile, deleteFile, format, checkIntegrity.
- Ассоциации: Directory, File.

**Directory**

- Поля (4): name, path, entries, parent.
- Методы (4): addFile, removeFile, listContents, findFile.
- Ассоциации: File, Directory.

**Kernel**

- Поля (3): version, modules, isRunning.
- Методы (5): boot, shutdown, loadModule, unloadModule, scheduleProcess.
- Ассоциации: Driver, Process.

**Scheduler**

- Поля (4): algorithm, quantumMs, queue, currentProcessIndex.
- Методы (5): enqueue, dequeue, scheduleNext, preempt, calculatePriority.
- Ассоциации: Process.

**MemoryManager**

- Поля (4): totalMemoryGB, allocatedMemoryGB, pageSizeKB, ram.
- Методы (5): allocatePages, freePages, defragment, checkFragmentation, swapOut.
- Ассоциации: RAMModule.

**DeviceManager**

- Поля (3): devices, conflicts, isScanning.
- Методы (4): registerDevice, detectDevice, resolveConflict, scanDevices.
- Ассоциации: Component.

**Antivirus**

- Поля (3): signatureVersion, threatsFound, isActive.
- Методы (5): activate, deactivate, scanFile, quarantine, updateSignatures.
- Ассоциации: File.

**BackupService**

- Поля (4): backupPath, lastBackupTime, backupCount, source.
- Методы (4): createBackup, restoreBackup, verifyBackup, scheduleBackup.
- Ассоциации: FileSystem.

**UpdateService**

- Поля (3): currentVersion, availableVersion, isUpdateAvailable.
- Методы (4): checkForUpdates, installUpdate, rollback, notifyUser.
- Ассоциации: нет.

**Compiler**

- Поля (4): language, optimizationLevel, errorCount, warningCount.
- Методы (4): compile, link, reportErrors, optimize.
- Ассоциации: File.

**VirtualMachine**

- Поля (5): vmId, allocatedRamGB, allocatedCpuCores, isRunning, guestOS.
- Методы (5): startVM, stopVM, snapshot, restoreSnapshot, allocateResources.
- Ассоциации: OperatingSystem.

## 5. Собственные исключения

Все исключения наследуются от `std::exception` и переопределяют `what()`. Они объявлены в пространстве имён `exceptions` и находятся в каталоге `exceptions/`.

| Исключение | Назначение |
|---|---|
| BIOSCorruptedException | повреждение или несовместимость прошивки BIOS/UEFI |
| DeviceNotFoundException | запрошенное аппаратное устройство не найдено |
| DiskFullException | на накопителе закончилось свободное место |
| DriverCrashException | аварийное завершение драйвера устройства |
| HardwareIncompatibleException | аппаратная несовместимость компонентов |
| NetworkTimeoutException | истекло время ожидания сетевого соединения |
| OutOfMemoryException | недостаточно оперативной памяти для операции |
| OverheatingException | критический перегрев компонента |
| PowerFailureException | авария или нехватка мощности блока питания |
| ShortCircuitException | короткое замыкание в цепи питания |
| SocketMismatchException | несовпадение сокета процессора и материнской платы |
| UnauthorizedAccessException | попытка несанкционированного доступа |
| AuthenticationException | неверный пароль, блокировка или слабый новый пароль |
| FileOperationException | ошибка создания, удаления или изменения файла |
| InvalidConfigurationException | некорректные параметры конфигурации системы |

## 6. Основные сценарии демонстрации

В `main.cpp` последовательно показаны:

1. Сборка компьютера: корпус, БП, BIOS, материнская плата, процессор, ОЗУ, накопители, видеокарта и охлаждение.
2. Подключение периферии: монитор, клавиатура, мышь, принтер, сканер, веб-камера, микрофон, акустика, USB, Bluetooth, Wi-Fi, батарея, датчик температуры и тачпад.
3. Работа программной подсистемы: пользователь, сессия, файловая система, каталоги, ядро, драйверы, планировщик, менеджер памяти, диспетчер устройств, антивирус, резервное копирование, обновления, компилятор и виртуальная машина.
4. Сеть: маршрутизатор, firewall-правило, сетевые пакеты и Wi-Fi-адаптер.
5. Обработка собственных исключений: `AuthenticationException` при трёх неверных паролях и `OverheatingException` при повторном разгоне процессора.

## 7. Структура репозитория

```text
hardware/   — комплектующие и периферийные устройства
network/    — сетевые пакеты, карты, маршрутизатор и firewall
software/   — ОС, процессы, файлы, пользователи, сервисы и утилиты
exceptions/ — собственные классы исключений
main.cpp    — демонстрационный сценарий
README.md   — описание объектной модели
```
