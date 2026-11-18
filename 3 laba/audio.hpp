#pragma once

#include "exceptions.hpp"
#include "util.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

namespace mp {

class AudioFormat {
    std::string name;
    std::string extension;
    bool lossless = false;
    int maxBitrateKbps = 320;

public:
    AudioFormat() = default;

    AudioFormat(std::string name, std::string extension, bool lossless, int maxBitrateKbps)
        : name(std::move(name)),
          extension(std::move(extension)),
          lossless(lossless),
          maxBitrateKbps(maxBitrateKbps) {}

    bool supportsBitrate(int bitrate) const {
        return bitrate > 0 && bitrate <= maxBitrateKbps;
    }

    bool isLossless() const {
        return lossless;
    }

    std::string describe() const {
        return name + " (." + extension + ", " + (lossless ? "lossless" : "lossy") + ", max " +
               std::to_string(maxBitrateKbps) + " kbps)";
    }

    std::string buildFileName(const std::string& title) const {
        std::string file = title;
        std::replace(file.begin(), file.end(), ' ', '_');
        return file + "." + extension;
    }

    const std::string& getName() const { return name; }
    const std::string& getExtension() const { return extension; }
    int getMaxBitrateKbps() const { return maxBitrateKbps; }
};

class AudioFile {
    std::string path;
    long long sizeBytes = 0;
    AudioFormat format;
    int bitrateKbps = 0;
    int sampleRateHz = 44100;

public:
    AudioFile() = default;

    AudioFile(std::string path, long long sizeBytes, AudioFormat format, int bitrateKbps,
              int sampleRateHz)
        : path(std::move(path)),
          sizeBytes(sizeBytes),
          format(std::move(format)),
          bitrateKbps(bitrateKbps),
          sampleRateHz(sampleRateHz) {}

    void validate() const {
        if (path.empty()) {
            throw InvalidTrackDataException("Audio file path is empty");
        }
        if (bitrateKbps <= 0) {
            throw InvalidTrackDataException("Audio file bitrate must be positive");
        }
        if (!format.supportsBitrate(bitrateKbps)) {
            throw InvalidTrackDataException("Bitrate " + std::to_string(bitrateKbps) +
                                            " is not supported by " + format.getName());
        }
    }

    double sizeInMb() const {
        return static_cast<double>(sizeBytes) / (1024.0 * 1024.0);
    }

    bool isHighQuality() const {
        return bitrateKbps >= 256 && sampleRateHz >= 44100;
    }

    AudioFile convertTo(const AudioFormat& target, int newBitrate) const {
        validate();
        if (newBitrate <= 0) {
            newBitrate = std::min(bitrateKbps, target.getMaxBitrateKbps());
        }
        if (!target.supportsBitrate(newBitrate)) {
            throw InvalidTrackDataException("Target format does not support requested bitrate");
        }

        const std::size_t dot = path.find_last_of('.');
        const std::string base = (dot == std::string::npos) ? path : path.substr(0, dot);
        return AudioFile(base + "." + target.getExtension(), sizeBytes, target, newBitrate,
                         sampleRateHz);
    }

    std::string describe() const {
        return path + " [" + format.getName() + ", " + std::to_string(bitrateKbps) + " kbps, " +
               std::to_string(sampleRateHz) + " Hz]";
    }

    const std::string& getPath() const { return path; }
    long long getSizeBytes() const { return sizeBytes; }
    const AudioFormat& getFormat() const { return format; }
    int getBitrateKbps() const { return bitrateKbps; }
    int getSampleRateHz() const { return sampleRateHz; }
};

class LyricLine {
    long long timeMs = 0;
    std::string content;
    std::string translation;

public:
    LyricLine() = default;

    LyricLine(long long timeMs, std::string content, std::string translation = "")
        : timeMs(timeMs), content(std::move(content)), translation(std::move(translation)) {}

    bool matches(long long timestampMs, int toleranceMs = 400) const {
        return std::llabs(timestampMs - timeMs) <= toleranceMs;
    }

    void translate(const std::string& value) {
        translation = value;
    }

    std::string display(bool withTranslation) const {
        if (withTranslation && !translation.empty()) {
            return content + " / " + translation;
        }
        return content;
    }

    bool isEmpty() const {
        return content.empty();
    }

    long long getTimeMs() const { return timeMs; }
    const std::string& getContent() const { return content; }
    const std::string& getTranslation() const { return translation; }
};

class Lyrics {
    std::vector<LyricLine> lines;
    std::string language;
    std::string source;
    bool synced = false;

public:
    Lyrics() = default;

    Lyrics(std::string language, std::string source, bool synced)
        : language(std::move(language)), source(std::move(source)), synced(synced) {}

    void addLine(const LyricLine& line) {
        lines.push_back(line);
        std::sort(lines.begin(), lines.end(), [](const LyricLine& left, const LyricLine& right) {
            return left.getTimeMs() < right.getTimeMs();
        });
        synced = true;
    }

    std::string textAt(long long timestampMs) const {
        const LyricLine* closest = nullptr;
        for (const LyricLine& line : lines) {
            if (line.matches(timestampMs, 1500)) {
                closest = &line;
                if (line.getTimeMs() >= timestampMs) {
                    break;
                }
            }
        }
        if (closest != nullptr) {
            return closest->display(false);
        }
        return "";
    }

    bool hasTranslation() const {
        return std::any_of(lines.begin(), lines.end(), [](const LyricLine& line) {
            return !line.getTranslation().empty();
        });
    }

    double translationCoverage() const {
        if (lines.empty()) {
            return 0.0;
        }
        const auto translated = std::count_if(lines.begin(), lines.end(), [](const LyricLine& line) {
            return !line.getTranslation().empty();
        });
        return 100.0 * static_cast<double>(translated) / static_cast<double>(lines.size());
    }

    void clear() {
        lines.clear();
        synced = false;
    }

    const std::vector<LyricLine>& getLines() const { return lines; }
    const std::string& getLanguage() const { return language; }
    const std::string& getSource() const { return source; }
    bool isSynced() const { return synced; }
};

class Genre {
    std::string name;
    std::string description;
    std::string colorHex;
    std::string parentName;

public:
    Genre() = default;

    Genre(std::string name, std::string description, std::string colorHex,
          std::string parentName = "")
        : name(std::move(name)),
          description(std::move(description)),
          colorHex(std::move(colorHex)),
          parentName(std::move(parentName)) {}

    bool isSubgenreOf(const Genre& other) const {
        return name != other.name && parentName == other.name;
    }

    bool matches(const std::string& query) const {
        return util::containsIgnoreCase(name, query) ||
               util::containsIgnoreCase(description, query);
    }

    std::string badge() const {
        return "[" + colorHex + "] " + name;
    }

    std::string hierarchy() const {
        return parentName.empty() ? name : parentName + " > " + name;
    }

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    const std::string& getColorHex() const { return colorHex; }
    const std::string& getParentName() const { return parentName; }
};

class Track {
    int id = 0;
    std::string title;
    std::string artistName;
    int durationSeconds = 0;
    int trackNumber = 0;
    AudioFile file;
    Genre genre;
    int playCount = 0;
    int rating = 0;
    bool explicitContent = false;

public:
    Track() = default;

    Track(int id, std::string title, std::string artistName, int durationSeconds, AudioFile file,
          Genre genre, int trackNumber = 1, bool explicitContent = false)
        : id(id),
          title(std::move(title)),
          artistName(std::move(artistName)),
          durationSeconds(durationSeconds),
          trackNumber(trackNumber),
          file(std::move(file)),
          genre(std::move(genre)),
          explicitContent(explicitContent) {}

    void validate() const {
        if (id <= 0) {
            throw InvalidTrackDataException("Track id must be positive");
        }
        if (title.empty()) {
            throw InvalidTrackDataException("Track title is empty");
        }
        if (durationSeconds <= 0) {
            throw InvalidTrackDataException("Track duration must be positive");
        }
        if (rating < 0 || rating > 5) {
            throw InvalidTrackDataException("Track rating must be in range 0..5");
        }
        file.validate();
    }

    int play() {
        validate();
        ++playCount;
        return durationSeconds;
    }

    void rate(int stars) {
        if (stars < 1 || stars > 5) {
            throw InvalidTrackDataException("Rating must be between 1 and 5");
        }
        rating = stars;
    }

    bool isLong() const {
        return durationSeconds > 360;
    }

    std::string formattedDuration() const {
        return util::formatDuration(durationSeconds);
    }

    bool matches(const std::string& query) const {
        return util::containsIgnoreCase(title, query) ||
               util::containsIgnoreCase(artistName, query) ||
               genre.matches(query);
    }

    void mergeMetadata(const Track& other) {
        if (title.empty()) {
            title = other.title;
        }
        if (artistName.empty()) {
            artistName = other.artistName;
        }
        if (durationSeconds <= 0) {
            durationSeconds = other.durationSeconds;
        }
        if (rating == 0) {
            rating = other.rating;
        }
        if (genre.getName().empty()) {
            genre = other.genre;
        }
    }

    int getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getArtistName() const { return artistName; }
    int getDurationSeconds() const { return durationSeconds; }
    int getTrackNumber() const { return trackNumber; }
    const AudioFile& getFile() const { return file; }
    const Genre& getGenre() const { return genre; }
    int getPlayCount() const { return playCount; }
    int getRating() const { return rating; }
    bool isExplicitContent() const { return explicitContent; }
};

class Album {
    std::string title;
    int releaseYear = 0;
    std::string label;
    std::string coverPath;
    Genre genre;
    std::vector<Track> tracks;

public:
    Album() = default;

    Album(std::string title, int releaseYear, std::string label, std::string coverPath,
          Genre genre)
        : title(std::move(title)),
          releaseYear(releaseYear),
          label(std::move(label)),
          coverPath(std::move(coverPath)),
          genre(std::move(genre)) {}

    void addTrack(const Track& track) {
        const bool exists = std::any_of(tracks.begin(), tracks.end(), [&](const Track& item) {
            return item.getId() == track.getId();
        });
        if (exists) {
            throw DuplicateTrackException("Track " + std::to_string(track.getId()) +
                                          " is already in album " + title);
        }
        tracks.push_back(track);
    }

    void removeTrack(int trackId) {
        const auto it = std::find_if(tracks.begin(), tracks.end(), [&](const Track& track) {
            return track.getId() == trackId;
        });
        if (it == tracks.end()) {
            throw TrackNotFoundException("Track " + std::to_string(trackId) +
                                         " is not in album " + title);
        }
        tracks.erase(it);
    }

    int totalDuration() const {
        int total = 0;
        for (const Track& track : tracks) {
            total += track.getDurationSeconds();
        }
        return total;
    }

    double averageRating() const {
        if (tracks.empty()) {
            return 0.0;
        }
        int sum = 0;
        int rated = 0;
        for (const Track& track : tracks) {
            if (track.getRating() > 0) {
                sum += track.getRating();
                ++rated;
            }
        }
        return rated == 0 ? 0.0 : static_cast<double>(sum) / rated;
    }

    bool isComplete(int expectedTracks) const {
        return static_cast<int>(tracks.size()) >= expectedTracks;
    }

    const Track* findTrack(const std::string& query) const {
        for (const Track& track : tracks) {
            if (track.matches(query)) {
                return &track;
            }
        }
        return nullptr;
    }

    const std::string& getTitle() const { return title; }
    int getReleaseYear() const { return releaseYear; }
    const std::string& getLabel() const { return label; }
    const std::string& getCoverPath() const { return coverPath; }
    const Genre& getGenre() const { return genre; }
    const std::vector<Track>& getTracks() const { return tracks; }
};

class Artist {
    std::string name;
    std::string country;
    Genre genre;
    long long monthlyListeners = 0;
    bool verified = false;

public:
    Artist() = default;

    Artist(std::string name, std::string country, Genre genre, long long monthlyListeners = 0)
        : name(std::move(name)),
          country(std::move(country)),
          genre(std::move(genre)),
          monthlyListeners(monthlyListeners) {}

    bool verify() {
        if (monthlyListeners < 1000) {
            return false;
        }
        verified = true;
        return true;
    }

    void addMonthlyListeners(long long count) {
        if (count < 0) {
            throw InvalidTrackDataException("Monthly listeners delta cannot be negative");
        }
        monthlyListeners += count;
    }

    bool matches(const std::string& query) const {
        return util::containsIgnoreCase(name, query) ||
               util::containsIgnoreCase(country, query) || genre.matches(query);
    }

    std::string displayName() const {
        return verified ? name + " [verified]" : name;
    }

    std::string rank() const {
        if (monthlyListeners >= 1000000) {
            return "global star";
        }
        if (monthlyListeners >= 100000) {
            return "popular";
        }
        if (monthlyListeners >= 1000) {
            return "growing";
        }
        return "underground";
    }

    const std::string& getName() const { return name; }
    const std::string& getCountry() const { return country; }
    const Genre& getGenre() const { return genre; }
    long long getMonthlyListeners() const { return monthlyListeners; }
    bool isVerified() const { return verified; }
};

class AudioAnalyzer {
    int bpm = 0;
    std::string musicalKey;
    double energy = 0.0;
    double loudnessDb = 0.0;

public:
    AudioAnalyzer() = default;

    void analyze(const Track& track) {
        track.validate();
        bpm = 70 + (track.getId() * 13) % 90;
        static const char* keys[] = {"C", "C#", "D", "D#", "E", "F",
                                     "F#", "G", "G#", "A", "A#", "B"};
        musicalKey = keys[track.getId() % 12];
        energy = std::min(1.0, track.getDurationSeconds() / 300.0 + track.getRating() / 10.0);
        loudnessDb = -20.0 + track.getRating() * 1.5;
    }

    int detectBpm(const std::vector<int>& beatIntervalsMs) {
        if (beatIntervalsMs.empty()) {
            throw InvalidTrackDataException("No beat intervals to analyze");
        }
        long long sum = 0;
        for (int interval : beatIntervalsMs) {
            if (interval <= 0) {
                throw InvalidTrackDataException("Beat interval must be positive");
            }
            sum += interval;
        }
        const double average = static_cast<double>(sum) / beatIntervalsMs.size();
        bpm = static_cast<int>(std::round(60000.0 / average));
        return bpm;
    }

    std::string detectKey(const std::vector<int>& pitchClasses) {
        if (pitchClasses.empty()) {
            throw InvalidTrackDataException("No pitch classes to analyze");
        }
        static const char* keys[] = {"C", "C#", "D", "D#", "E", "F",
                                     "F#", "G", "G#", "A", "A#", "B"};
        int counts[12] = {0};
        for (int pitch : pitchClasses) {
            if (pitch < 0 || pitch > 11) {
                throw InvalidTrackDataException("Pitch class must be in range 0..11");
            }
            ++counts[pitch];
        }
        int best = 0;
        for (int i = 1; i < 12; ++i) {
            if (counts[i] > counts[best]) {
                best = i;
            }
        }
        musicalKey = keys[best];
        return musicalKey;
    }

    std::string energyLevel() const {
        if (energy >= 0.75) {
            return "high";
        }
        if (energy >= 0.4) {
            return "medium";
        }
        return "low";
    }

    bool isDanceable() const {
        return bpm >= 90 && bpm <= 130 && energy >= 0.5;
    }

    int getBpm() const { return bpm; }
    const std::string& getMusicalKey() const { return musicalKey; }
    double getEnergy() const { return energy; }
    double getLoudnessDb() const { return loudnessDb; }
};

} // namespace mp
