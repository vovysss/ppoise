#pragma once

#include "audio.hpp"
#include "exceptions.hpp"
#include "util.hpp"

#include <algorithm>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace mp {

class EqualizerBand {
    int frequencyHz = 0;
    double gainDb = 0.0;
    double minGainDb = -12.0;
    double maxGainDb = 12.0;

public:
    EqualizerBand() = default;

    EqualizerBand(int frequencyHz, double gainDb = 0.0, double minGainDb = -12.0,
                  double maxGainDb = 12.0)
        : frequencyHz(frequencyHz),
          gainDb(gainDb),
          minGainDb(minGainDb),
          maxGainDb(maxGainDb) {}

    void adjustGain(double deltaDb) {
        gainDb = std::max(minGainDb, std::min(maxGainDb, gainDb + deltaDb));
    }

    void reset() {
        gainDb = 0.0;
    }

    bool isBoosted() const {
        return gainDb > 0.0;
    }

    std::string describe() const {
        return std::to_string(frequencyHz) + " Hz: " + std::to_string(gainDb) + " dB";
    }

    int getFrequencyHz() const { return frequencyHz; }
    double getGainDb() const { return gainDb; }
    double getMinGainDb() const { return minGainDb; }
    double getMaxGainDb() const { return maxGainDb; }
};

class Equalizer {
    std::vector<EqualizerBand> bands;
    std::string presetName = "Flat";
    bool enabled = false;

public:
    Equalizer() {
        bands.emplace_back(60);
        bands.emplace_back(230);
        bands.emplace_back(910);
        bands.emplace_back(3600);
        bands.emplace_back(14000);
    }

    void adjustBand(std::size_t index, double deltaDb) {
        if (index >= bands.size()) {
            throw MusicPlayerException("Equalizer band index is out of range");
        }
        bands[index].adjustGain(deltaDb);
    }

    void applyPreset(const std::string& name) {
        static const std::map<std::string, std::vector<double>> presets = {
            {"Flat", {0, 0, 0, 0, 0}},
            {"Bass Boost", {6, 4, 0, -1, -2}},
            {"Vocal", {-2, 0, 3, 4, 2}},
            {"Rock", {4, 2, -1, 3, 5}},
            {"Acoustic", {3, 1, 2, 4, 1}},
        };

        const auto it = presets.find(name);
        if (it == presets.end()) {
            throw MusicPlayerException("Unknown equalizer preset: " + name);
        }
        for (std::size_t i = 0; i < bands.size() && i < it->second.size(); ++i) {
            bands[i].adjustGain(it->second[i] - bands[i].getGainDb());
        }
        presetName = name;
        enabled = true;
    }

    void reset() {
        for (EqualizerBand& band : bands) {
            band.reset();
        }
        presetName = "Flat";
        enabled = false;
    }

    bool toggle() {
        enabled = !enabled;
        return enabled;
    }

    double averageGain() const {
        if (bands.empty()) {
            return 0.0;
        }
        double sum = 0.0;
        for (const EqualizerBand& band : bands) {
            sum += band.getGainDb();
        }
        return sum / static_cast<double>(bands.size());
    }

    const std::vector<EqualizerBand>& getBands() const { return bands; }
    const std::string& getPresetName() const { return presetName; }
    bool isEnabled() const { return enabled; }
};

class StreamQuality {
    int bitrateKbps = 128;
    std::string codec = "AAC";
    int sampleRateHz = 44100;

public:
    StreamQuality() = default;

    StreamQuality(int bitrateKbps, std::string codec, int sampleRateHz)
        : bitrateKbps(bitrateKbps),
          codec(std::move(codec)),
          sampleRateHz(sampleRateHz) {}

    bool isLossless() const {
        return codec == "FLAC" || codec == "ALAC" || codec == "WAV";
    }

    void upgrade(int newBitrateKbps) {
        if (newBitrateKbps <= bitrateKbps) {
            throw MusicPlayerException("New bitrate must be higher than current");
        }
        bitrateKbps = newBitrateKbps;
    }

    std::string describe() const {
        return codec + " " + std::to_string(bitrateKbps) + " kbps / " +
               std::to_string(sampleRateHz) + " Hz" + (isLossless() ? " (lossless)" : "");
    }

    int getBitrateKbps() const { return bitrateKbps; }
    const std::string& getCodec() const { return codec; }
    int getSampleRateHz() const { return sampleRateHz; }
};

class AudioDevice {
    std::string name;
    std::string type;
    bool connected = true;
    int volumeLevel = 50;
    int latencyMs = 20;
    int batteryPercent = 100;

public:
    AudioDevice() = default;

    AudioDevice(std::string name, std::string type, int volumeLevel = 50, int latencyMs = 20)
        : name(std::move(name)),
          type(std::move(type)),
          volumeLevel(volumeLevel),
          latencyMs(latencyMs) {}

    void connect() {
        connected = true;
    }

    void disconnect() {
        connected = false;
    }

    void changeVolume(int value) {
        if (value < 0 || value > 100) {
            throw InvalidVolumeException("Volume must be in range 0..100");
        }
        volumeLevel = value;
    }

    bool testLatency() const {
        return connected && latencyMs <= 100;
    }

    bool isWireless() const {
        return type == "Bluetooth" || type == "AirPlay" || type == "Wi-Fi";
    }

    std::string status() const {
        return name + " (" + type + "): " + (connected ? "connected" : "disconnected") +
               ", volume " + std::to_string(volumeLevel) + "%, latency " +
               std::to_string(latencyMs) + " ms";
    }

    const std::string& getName() const { return name; }
    const std::string& getType() const { return type; }
    bool isConnected() const { return connected; }
    int getVolumeLevel() const { return volumeLevel; }
    int getLatencyMs() const { return latencyMs; }
    int getBatteryPercent() const { return batteryPercent; }
};

class Headphones : public AudioDevice {
    bool noiseCancellation = false;
    double batteryHours = 20.0;

public:
    Headphones() = default;

    Headphones(std::string name, std::string type, double batteryHours)
        : AudioDevice(std::move(name), std::move(type), 50, 15), batteryHours(batteryHours) {}

    void enableAnc() {
        if (!isWireless()) {
            throw AudioDeviceException("Noise cancellation requires a wireless device");
        }
        noiseCancellation = true;
    }

    void disableAnc() {
        noiseCancellation = false;
    }

    void charge(int minutes) {
        if (minutes <= 0) {
            throw AudioDeviceException("Charge duration must be positive");
        }
        batteryHours = std::min(40.0, batteryHours + minutes / 60.0);
    }

    std::string ancStatus() const {
        return noiseCancellation ? "ANC on" : "ANC off";
    }

    bool isNoiseCancellation() const { return noiseCancellation; }
    double getBatteryHours() const { return batteryHours; }
};

class Speaker : public AudioDevice {
    int channels = 2;
    int wattage = 20;

public:
    Speaker() = default;

    Speaker(std::string name, int channels, int wattage)
        : AudioDevice(std::move(name), "Speaker", 60, 10), channels(channels), wattage(wattage) {}

    void chooseStereo(bool stereo) {
        channels = stereo ? 2 : 1;
    }

    int powerRating() const {
        return channels * wattage;
    }

    std::string describe() const {
        return getName() + ": " + std::to_string(channels) + " channel(s), " +
               std::to_string(wattage) + " W per channel";
    }

    int getChannels() const { return channels; }
    int getWattage() const { return wattage; }
};

class PlaybackQueue {
    std::vector<Track> tracks;
    int currentIndex = -1;
    bool shuffleEnabled = false;
    std::string repeatMode = "off";

public:
    PlaybackQueue() = default;

    void enqueue(const Track& track) {
        track.validate();
        tracks.push_back(track);
        if (currentIndex < 0) {
            currentIndex = 0;
        }
    }

    void removeAt(std::size_t index) {
        if (tracks.empty()) {
            throw EmptyQueueException("Cannot remove from an empty playback queue");
        }
        if (index >= tracks.size()) {
            throw MusicPlayerException("Queue index is out of range");
        }
        tracks.erase(tracks.begin() + static_cast<long>(index));
        if (tracks.empty()) {
            currentIndex = -1;
        } else if (currentIndex >= static_cast<int>(tracks.size())) {
            currentIndex = static_cast<int>(tracks.size()) - 1;
        }
    }

    const Track* next() {
        if (tracks.empty()) {
            throw EmptyQueueException("Playback queue is empty");
        }
        if (repeatMode == "one") {
            return &tracks[currentIndex];
        }
        if (currentIndex + 1 < static_cast<int>(tracks.size())) {
            ++currentIndex;
            return &tracks[currentIndex];
        }
        if (repeatMode == "all") {
            currentIndex = 0;
            return &tracks[currentIndex];
        }
        return nullptr;
    }

    const Track* previous() {
        if (tracks.empty()) {
            throw EmptyQueueException("Playback queue is empty");
        }
        if (repeatMode == "one") {
            return &tracks[currentIndex];
        }
        if (currentIndex > 0) {
            --currentIndex;
            return &tracks[currentIndex];
        }
        if (repeatMode == "all") {
            currentIndex = static_cast<int>(tracks.size()) - 1;
            return &tracks[currentIndex];
        }
        return &tracks[currentIndex];
    }

    bool toggleShuffle() {
        shuffleEnabled = !shuffleEnabled;
        if (shuffleEnabled && tracks.size() > 1) {
            std::mt19937 generator(std::random_device{}());
            std::shuffle(tracks.begin(), tracks.end(), generator);
            currentIndex = 0;
        }
        return shuffleEnabled;
    }

    void chooseRepeatMode(const std::string& mode) {
        if (mode != "off" && mode != "all" && mode != "one") {
            throw MusicPlayerException("Repeat mode must be off, all or one");
        }
        repeatMode = mode;
    }

    void clear() {
        tracks.clear();
        currentIndex = -1;
    }

    const Track* current() const {
        if (tracks.empty() || currentIndex < 0 || currentIndex >= static_cast<int>(tracks.size())) {
            throw EmptyQueueException("Playback queue has no current track");
        }
        return &tracks[currentIndex];
    }

    const std::vector<Track>& getTracks() const { return tracks; }
    int getCurrentIndex() const { return currentIndex; }
    bool isShuffleEnabled() const { return shuffleEnabled; }
    const std::string& getRepeatMode() const { return repeatMode; }
    bool isEmpty() const { return tracks.empty(); }
};

class PlaybackSession {
    Track track;
    std::time_t startedAt = 0;
    int listenedSeconds = 0;
    bool completed = false;
    std::string deviceName;
    bool paused = false;

public:
    PlaybackSession() = default;

    PlaybackSession(Track track, std::string deviceName)
        : track(std::move(track)),
          startedAt(util::now()),
          deviceName(std::move(deviceName)) {}

    void pause() {
        paused = true;
    }

    void resume() {
        paused = false;
    }

    void finish(int seconds) {
        listenedSeconds = std::max(0, seconds);
        completed = completionPercent() >= 95.0;
        paused = false;
    }

    double completionPercent() const {
        if (track.getDurationSeconds() <= 0) {
            return 0.0;
        }
        return std::min(100.0, 100.0 * listenedSeconds / track.getDurationSeconds());
    }

    bool isCompleted() const {
        return completed;
    }

    std::string summary() const {
        return track.getTitle() + " on " + deviceName + ": " +
               util::formatDuration(listenedSeconds) + " (" +
               std::to_string(static_cast<int>(completionPercent())) + "%)";
    }

    const Track& getTrack() const { return track; }
    std::time_t getStartedAt() const { return startedAt; }
    int getListenedSeconds() const { return listenedSeconds; }
    const std::string& getDeviceName() const { return deviceName; }
    bool isPaused() const { return paused; }
};

class PlaybackHistory {
    std::vector<PlaybackSession> sessions;
    std::size_t maxEntries = 500;
    long long totalListeningSeconds = 0;

public:
    PlaybackHistory() = default;

    explicit PlaybackHistory(std::size_t maxEntries) : maxEntries(maxEntries) {}

    void record(const PlaybackSession& session) {
        sessions.push_back(session);
        totalListeningSeconds += session.getListenedSeconds();
        while (sessions.size() > maxEntries) {
            sessions.erase(sessions.begin());
        }
    }

    std::vector<PlaybackSession> recent(int count) const {
        std::vector<PlaybackSession> result;
        for (auto it = sessions.rbegin(); it != sessions.rend() && static_cast<int>(result.size()) < count; ++it) {
            result.push_back(*it);
        }
        return result;
    }

    int countPlays(int trackId) const {
        return static_cast<int>(std::count_if(sessions.begin(), sessions.end(),
                                             [&](const PlaybackSession& session) {
                                                 return session.getTrack().getId() == trackId;
                                             }));
    }

    int mostPlayedTrackId() const {
        std::map<int, int> counts;
        for (const PlaybackSession& session : sessions) {
            ++counts[session.getTrack().getId()];
        }
        int bestId = -1;
        int bestCount = -1;
        for (const auto& entry : counts) {
            if (entry.second > bestCount) {
                bestCount = entry.second;
                bestId = entry.first;
            }
        }
        return bestId;
    }

    void clear() {
        sessions.clear();
        totalListeningSeconds = 0;
    }

    const std::vector<PlaybackSession>& getSessions() const { return sessions; }
    std::size_t getMaxEntries() const { return maxEntries; }
    long long getTotalListeningSeconds() const { return totalListeningSeconds; }
};

class Player {
    PlaybackQueue queue;
    std::string state = "stopped";
    int volume = 50;
    int positionSeconds = 0;
    double playbackSpeed = 1.0;
    Equalizer equalizer;
    AudioDevice* device = nullptr;
    PlaybackHistory* history = nullptr;
    bool muted = false;
    int crossfadeSeconds = 0;

public:
    Player() = default;

    Player(PlaybackQueue queue, Equalizer equalizer, AudioDevice* device,
           PlaybackHistory* history)
        : queue(std::move(queue)),
          equalizer(std::move(equalizer)),
          device(device),
          history(history) {}

    const Track& play() {
        if (device != nullptr && !device->isConnected()) {
            throw DeviceNotConnectedException("Cannot play: audio device is disconnected");
        }
        const Track* track = queue.current();
        state = "playing";
        return *track;
    }

    void pause() {
        if (state != "playing") {
            throw MusicPlayerException("Pause is available only during playback");
        }
        state = "paused";
    }

    void resume() {
        if (state != "paused") {
            throw MusicPlayerException("Resume is available only after pause");
        }
        state = "playing";
    }

    void stop() {
        if (history != nullptr && !queue.isEmpty()) {
            PlaybackSession session(*queue.current(), device != nullptr ? device->getName() : "unknown");
            session.finish(positionSeconds);
            history->record(session);
        }
        state = "stopped";
        positionSeconds = 0;
    }

    const Track* next() {
        const Track* track = queue.next();
        positionSeconds = 0;
        if (track == nullptr) {
            state = "stopped";
        } else {
            state = "playing";
        }
        return track;
    }

    const Track* previous() {
        const Track* track = queue.previous();
        positionSeconds = 0;
        state = "playing";
        return track;
    }

    void seek(int seconds) {
        const Track& track = *queue.current();
        positionSeconds = std::max(0, std::min(seconds, track.getDurationSeconds()));
    }

    void changeVolume(int value) {
        if (value < 0 || value > 100) {
            throw InvalidVolumeException("Volume must be in range 0..100");
        }
        volume = value;
        muted = false;
    }

    bool toggleMute() {
        muted = !muted;
        return muted;
    }

    void changeSpeed(double value) {
        if (value < 0.5 || value > 2.0) {
            throw MusicPlayerException("Playback speed must be in range 0.5..2.0");
        }
        playbackSpeed = value;
    }

    void applyEqualizer(const std::string& preset) {
        equalizer.applyPreset(preset);
    }

    void listenFor(int seconds) {
        if (state != "playing") {
            throw MusicPlayerException("Playback is not active");
        }
        if (seconds < 0) {
            throw MusicPlayerException("Listening duration cannot be negative");
        }
        positionSeconds += seconds;
        const Track* current = queue.current();
        if (positionSeconds >= current->getDurationSeconds()) {
            positionSeconds = 0;
            next();
        }
    }

    std::string currentTitle() const {
        if (queue.isEmpty()) {
            return "nothing";
        }
        return queue.current()->getTitle();
    }

    const PlaybackQueue& getQueue() const { return queue; }
    const std::string& getState() const { return state; }
    int getVolume() const { return volume; }
    int getPositionSeconds() const { return positionSeconds; }
    double getPlaybackSpeed() const { return playbackSpeed; }
    const Equalizer& getEqualizer() const { return equalizer; }
    bool isMuted() const { return muted; }
    int getCrossfadeSeconds() const { return crossfadeSeconds; }
    void setHistory(PlaybackHistory* value) { history = value; }
    void setDevice(AudioDevice* value) { device = value; }
};

class SleepTimer {
    int durationMinutes = 0;
    int remainingSeconds = 0;
    bool active = false;
    bool fadeOut = true;

public:
    SleepTimer() = default;

    void start(int minutes) {
        if (minutes <= 0) {
            throw MusicPlayerException("Sleep timer duration must be positive");
        }
        durationMinutes = minutes;
        remainingSeconds = minutes * 60;
        active = true;
    }

    void tick(int seconds) {
        if (!active) {
            return;
        }
        remainingSeconds -= seconds;
        if (remainingSeconds <= 0) {
            remainingSeconds = 0;
            active = false;
        }
    }

    void cancel() {
        active = false;
        remainingSeconds = 0;
    }

    bool isExpired() const {
        return durationMinutes > 0 && remainingSeconds == 0 && !active;
    }

    std::string remainingFormatted() const {
        return util::formatDuration(remainingSeconds);
    }

    int getDurationMinutes() const { return durationMinutes; }
    int getRemainingSeconds() const { return remainingSeconds; }
    bool isActive() const { return active; }
    bool isFadeOut() const { return fadeOut; }
    void setFadeOut(bool value) { fadeOut = value; }
};

} // namespace mp
