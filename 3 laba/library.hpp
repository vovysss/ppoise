#pragma once

#include "audio.hpp"
#include "exceptions.hpp"
#include "util.hpp"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace mp {

class PlaylistEntry {
    Track track;
    int position = 1;
    std::time_t addedAt = 0;
    std::string addedBy;

public:
    PlaylistEntry() = default;

    PlaylistEntry(Track track, int position, std::time_t addedAt, std::string addedBy)
        : track(std::move(track)),
          position(position),
          addedAt(addedAt),
          addedBy(std::move(addedBy)) {}

    void moveUp() {
        if (position > 1) {
            --position;
        }
    }

    void moveDown() {
        ++position;
    }

    void updatePosition(int newPosition) {
        if (newPosition <= 0) {
            throw MusicPlayerException("Playlist position must be positive");
        }
        position = newPosition;
    }

    std::string describe() const {
        return "#" + std::to_string(position) + " " + track.getTitle() + " by " + addedBy;
    }

    const Track& getTrack() const { return track; }
    int getPosition() const { return position; }
    std::time_t getAddedAt() const { return addedAt; }
    const std::string& getAddedBy() const { return addedBy; }
};

class Playlist {
    std::string name;
    std::string description;
    bool isPublic = false;
    std::time_t createdAt = 0;
    std::vector<PlaylistEntry> entries;
    int playCount = 0;

public:
    Playlist() = default;

    Playlist(std::string name, std::string description, bool isPublic)
        : name(std::move(name)),
          description(std::move(description)),
          isPublic(isPublic),
          createdAt(util::now()) {}

    void addTrack(const Track& track, const std::string& user) {
        track.validate();
        if (entries.size() >= 100) {
            throw PlaylistLimitException("Playlist cannot contain more than 100 tracks");
        }
        if (contains(track.getId())) {
            throw DuplicateTrackException("Track " + std::to_string(track.getId()) +
                                          " is already in playlist " + name);
        }
        entries.emplace_back(track, static_cast<int>(entries.size()) + 1, util::now(), user);
    }

    void removeTrack(int trackId) {
        const auto it = std::find_if(entries.begin(), entries.end(), [&](const PlaylistEntry& entry) {
            return entry.getTrack().getId() == trackId;
        });
        if (it == entries.end()) {
            throw TrackNotFoundException("Track " + std::to_string(trackId) +
                                         " is not in playlist " + name);
        }
        entries.erase(it);
        for (std::size_t i = 0; i < entries.size(); ++i) {
            entries[i].updatePosition(static_cast<int>(i) + 1);
        }
    }

    void moveTrack(int fromPosition, int toPosition) {
        if (fromPosition < 1 || fromPosition > static_cast<int>(entries.size()) ||
            toPosition < 1 || toPosition > static_cast<int>(entries.size())) {
            throw MusicPlayerException("Playlist position is out of range");
        }
        PlaylistEntry moved = entries[fromPosition - 1];
        entries.erase(entries.begin() + (fromPosition - 1));
        entries.insert(entries.begin() + (toPosition - 1), moved);
        for (std::size_t i = 0; i < entries.size(); ++i) {
            entries[i].updatePosition(static_cast<int>(i) + 1);
        }
    }

    int totalDuration() const {
        int total = 0;
        for (const PlaylistEntry& entry : entries) {
            total += entry.getTrack().getDurationSeconds();
        }
        return total;
    }

    bool contains(int trackId) const {
        return std::any_of(entries.begin(), entries.end(), [&](const PlaylistEntry& entry) {
            return entry.getTrack().getId() == trackId;
        });
    }

    void shuffle() {
        if (entries.size() < 2) {
            return;
        }
        std::mt19937 generator(std::random_device{}());
        std::shuffle(entries.begin(), entries.end(), generator);
        for (std::size_t i = 0; i < entries.size(); ++i) {
            entries[i].updatePosition(static_cast<int>(i) + 1);
        }
    }

    void clear() {
        entries.clear();
    }

    std::string shareLink() const {
        std::string slug;
        for (unsigned char c : name) {
            if (std::isalnum(c)) {
                slug += static_cast<char>(std::tolower(c));
            } else if (!slug.empty() && slug.back() != '-') {
                slug += '-';
            }
        }
        while (!slug.empty() && slug.back() == '-') {
            slug.pop_back();
        }
        return "musicplayer://playlist/" + slug;
    }

    void markPlayed() {
        ++playCount;
    }

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    bool isPublicPlaylist() const { return isPublic; }
    std::time_t getCreatedAt() const { return createdAt; }
    const std::vector<PlaylistEntry>& getEntries() const { return entries; }
    int getPlayCount() const { return playCount; }
};

class MusicLibrary {
    std::vector<Track> tracks;
    std::vector<Album> albums;
    std::vector<Artist> artists;
    std::vector<Playlist> playlists;
    std::time_t lastScanAt = 0;
    std::string storagePath;

public:
    MusicLibrary() = default;

    explicit MusicLibrary(std::string storagePath) : storagePath(std::move(storagePath)) {}

    void addTrack(const Track& track) {
        track.validate();
        const bool exists = std::any_of(tracks.begin(), tracks.end(), [&](const Track& item) {
            return item.getId() == track.getId();
        });
        if (exists) {
            throw DuplicateTrackException("Track " + std::to_string(track.getId()) +
                                          " already exists in library");
        }
        tracks.push_back(track);
    }

    void removeTrack(int trackId) {
        const auto it = std::find_if(tracks.begin(), tracks.end(), [&](const Track& track) {
            return track.getId() == trackId;
        });
        if (it == tracks.end()) {
            throw TrackNotFoundException("Track " + std::to_string(trackId) +
                                         " was not found in library");
        }
        tracks.erase(it);
    }

    void addAlbum(const Album& album) {
        albums.push_back(album);
        lastScanAt = util::now();
    }

    void addArtist(const Artist& artist) {
        artists.push_back(artist);
    }

    void addPlaylist(const Playlist& playlist) {
        playlists.push_back(playlist);
    }

    const Track* findTrackById(int trackId) const {
        for (const Track& track : tracks) {
            if (track.getId() == trackId) {
                return &track;
            }
        }
        return nullptr;
    }

    std::vector<Track> searchByTitle(const std::string& query) const {
        std::vector<Track> result;
        for (const Track& track : tracks) {
            if (util::containsIgnoreCase(track.getTitle(), query)) {
                result.push_back(track);
            }
        }
        return result;
    }

    std::string statistics() const {
        int duration = 0;
        for (const Track& track : tracks) {
            duration += track.getDurationSeconds();
        }
        return "tracks: " + std::to_string(tracks.size()) +
               ", albums: " + std::to_string(albums.size()) +
               ", artists: " + std::to_string(artists.size()) +
               ", playlists: " + std::to_string(playlists.size()) +
               ", duration: " + util::formatDuration(duration);
    }

    void clear() {
        tracks.clear();
        albums.clear();
        artists.clear();
        playlists.clear();
    }

    const std::vector<Track>& getTracks() const { return tracks; }
    const std::vector<Album>& getAlbums() const { return albums; }
    const std::vector<Artist>& getArtists() const { return artists; }
    const std::vector<Playlist>& getPlaylists() const { return playlists; }
    std::time_t getLastScanAt() const { return lastScanAt; }
    const std::string& getStoragePath() const { return storagePath; }
};

class SearchQuery {
    std::string text;
    std::string field = "any";
    bool caseSensitive = false;
    int limit = 20;
    Genre genre;

public:
    SearchQuery() = default;

    SearchQuery(std::string text, std::string field, bool caseSensitive, int limit, Genre genre)
        : text(std::move(text)),
          field(std::move(field)),
          caseSensitive(caseSensitive),
          limit(limit),
          genre(std::move(genre)) {}

    std::string normalize() const {
        return caseSensitive ? text : util::toLower(text);
    }

    bool isValid() const {
        return !text.empty() && limit > 0;
    }

    bool matches(const Track& track) const {
        if (!isValid()) {
            return false;
        }
        if (!genre.getName().empty() && track.getGenre().getName() != genre.getName()) {
            return false;
        }
        if (field == "title") {
            return util::containsWithCase(track.getTitle(), text, caseSensitive);
        }
        if (field == "artist") {
            return util::containsWithCase(track.getArtistName(), text, caseSensitive);
        }
        if (field == "genre") {
            return util::containsWithCase(track.getGenre().getName(), text, caseSensitive);
        }
        return util::containsWithCase(track.getTitle(), text, caseSensitive) ||
               util::containsWithCase(track.getArtistName(), text, caseSensitive) ||
               util::containsWithCase(track.getGenre().getName(), text, caseSensitive);
    }

    std::string describe() const {
        return "field=" + field + ", query=" + text + ", limit=" + std::to_string(limit) +
               (genre.getName().empty() ? "" : ", genre=" + genre.getName());
    }

    const std::string& getText() const { return text; }
    const std::string& getField() const { return field; }
    bool isCaseSensitive() const { return caseSensitive; }
    int getLimit() const { return limit; }
    const Genre& getGenre() const { return genre; }
};

class SearchEngine {
    const MusicLibrary* library = nullptr;
    std::vector<std::string> history;
    int maxResults = 100;

public:
    SearchEngine() = default;

    explicit SearchEngine(const MusicLibrary* library) : library(library) {}

    std::vector<Track> searchTracks(const SearchQuery& query) {
        std::vector<Track> result;
        if (library == nullptr || !query.isValid()) {
            return result;
        }
        const int limit = std::min(query.getLimit(), maxResults);
        for (const Track& track : library->getTracks()) {
            if (query.matches(track)) {
                result.push_back(track);
                if (static_cast<int>(result.size()) >= limit) {
                    break;
                }
            }
        }
        history.push_back(query.describe());
        return result;
    }

    std::vector<Playlist> searchPlaylists(const std::string& query) {
        std::vector<Playlist> result;
        if (library == nullptr) {
            return result;
        }
        for (const Playlist& playlist : library->getPlaylists()) {
            if (util::containsIgnoreCase(playlist.getName(), query)) {
                result.push_back(playlist);
            }
        }
        history.push_back("playlist:" + query);
        return result;
    }

    void sortByRelevance(std::vector<Track>& tracks) const {
        std::sort(tracks.begin(), tracks.end(), [](const Track& left, const Track& right) {
            if (left.getRating() != right.getRating()) {
                return left.getRating() > right.getRating();
            }
            return left.getPlayCount() > right.getPlayCount();
        });
    }

    std::vector<Track> searchByArtist(const std::string& artistName) {
        std::vector<Track> result;
        if (library == nullptr) {
            return result;
        }
        for (const Track& track : library->getTracks()) {
            if (util::containsIgnoreCase(track.getArtistName(), artistName)) {
                result.push_back(track);
            }
        }
        sortByRelevance(result);
        history.push_back("artist:" + artistName);
        return result;
    }

    void clearHistory() {
        history.clear();
    }

    const MusicLibrary* getLibrary() const { return library; }
    const std::vector<std::string>& getHistory() const { return history; }
    int getMaxResults() const { return maxResults; }
};

class MediaScanner {
    std::vector<std::string> supportedExtensions;
    int foundCount = 0;
    std::string lastScanPath;
    std::vector<std::string> ignoredPaths;

public:
    MediaScanner() {
        supportedExtensions = {"mp3", "flac", "wav", "aac", "ogg", "m4a"};
    }

    void registerFormat(const std::string& extension) {
        std::string normalized = util::toLower(extension);
        if (!normalized.empty() && normalized.front() == '.') {
            normalized.erase(normalized.begin());
        }
        if (normalized.empty()) {
            throw MusicPlayerException("File extension cannot be empty");
        }
        if (std::find(supportedExtensions.begin(), supportedExtensions.end(), normalized) ==
            supportedExtensions.end()) {
            supportedExtensions.push_back(normalized);
        }
    }

    int scanDirectory(const std::string& path, const std::vector<std::string>& files) {
        lastScanPath = path;
        int count = 0;
        for (const std::string& file : files) {
            const bool ignored = std::any_of(ignoredPaths.begin(), ignoredPaths.end(),
                                             [&](const std::string& ignoredPath) {
                                                 return file.find(ignoredPath) != std::string::npos;
                                             });
            if (!ignored && isSupported(file)) {
                ++count;
            }
        }
        foundCount += count;
        return count;
    }

    bool isSupported(const std::string& file) const {
        const std::string extension = util::extensionOf(file);
        return std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) !=
               supportedExtensions.end();
    }

    std::string report() const {
        return "scanned " + lastScanPath + ": " + std::to_string(foundCount) + " media files";
    }

    void ignorePath(const std::string& path) {
        ignoredPaths.push_back(path);
    }

    const std::vector<std::string>& getSupportedExtensions() const { return supportedExtensions; }
    int getFoundCount() const { return foundCount; }
    const std::string& getLastScanPath() const { return lastScanPath; }
    const std::vector<std::string>& getIgnoredPaths() const { return ignoredPaths; }
};

class CacheEntry {
    int trackId = 0;
    double sizeMb = 0.0;
    std::time_t lastAccessedAt = 0;
    int hits = 0;

public:
    CacheEntry() = default;

    CacheEntry(int trackId, double sizeMb, std::time_t lastAccessedAt, int hits = 0)
        : trackId(trackId),
          sizeMb(sizeMb),
          lastAccessedAt(lastAccessedAt),
          hits(hits) {}

    void touch() {
        lastAccessedAt = util::now();
        ++hits;
    }

    void increaseHits() {
        ++hits;
    }

    bool isExpired(int days, std::time_t currentTime) const {
        return days > 0 && currentTime - lastAccessedAt > static_cast<long>(days) * 86400L;
    }

    std::string describe() const {
        return "track " + std::to_string(trackId) + ", " + std::to_string(sizeMb) +
               " MB, hits " + std::to_string(hits);
    }

    int getTrackId() const { return trackId; }
    double getSizeMb() const { return sizeMb; }
    std::time_t getLastAccessedAt() const { return lastAccessedAt; }
    int getHits() const { return hits; }
};

class CacheManager {
    double maxSizeMb = 1024.0;
    double currentSizeMb = 0.0;
    std::vector<CacheEntry> entries;
    int hitCount = 0;
    int missCount = 0;

public:
    CacheManager() = default;

    explicit CacheManager(double maxSizeMb) : maxSizeMb(maxSizeMb) {}

    void cacheTrack(const Track& track, double sizeMb) {
        if (sizeMb <= 0) {
            throw MusicPlayerException("Cache entry size must be positive");
        }
        if (sizeMb > maxSizeMb) {
            throw StorageLimitException("Track is larger than the whole cache");
        }
        for (CacheEntry& entry : entries) {
            if (entry.getTrackId() == track.getId()) {
                entry.touch();
                return;
            }
        }
        while (currentSizeMb + sizeMb > maxSizeMb && !entries.empty()) {
            evictOldest();
        }
        entries.emplace_back(track.getId(), sizeMb, util::now());
        currentSizeMb += sizeMb;
    }

    bool evictOldest() {
        if (entries.empty()) {
            return false;
        }
        auto oldest = entries.begin();
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->getLastAccessedAt() < oldest->getLastAccessedAt()) {
                oldest = it;
            }
        }
        currentSizeMb -= oldest->getSizeMb();
        entries.erase(oldest);
        return true;
    }

    void clear() {
        entries.clear();
        currentSizeMb = 0.0;
        hitCount = 0;
        missCount = 0;
    }

    bool isCached(int trackId) {
        for (CacheEntry& entry : entries) {
            if (entry.getTrackId() == trackId) {
                entry.touch();
                ++hitCount;
                return true;
            }
        }
        ++missCount;
        return false;
    }

    double hitRate() const {
        const int total = hitCount + missCount;
        return total == 0 ? 0.0 : 100.0 * hitCount / total;
    }

    int optimize() {
        const std::time_t currentTime = util::now();
        const std::size_t before = entries.size();
        entries.erase(std::remove_if(entries.begin(), entries.end(),
                                     [&](const CacheEntry& entry) {
                                         return entry.isExpired(30, currentTime);
                                     }),
                      entries.end());
        currentSizeMb = 0.0;
        for (const CacheEntry& entry : entries) {
            currentSizeMb += entry.getSizeMb();
        }
        return static_cast<int>(before - entries.size());
    }

    double getMaxSizeMb() const { return maxSizeMb; }
    double getCurrentSizeMb() const { return currentSizeMb; }
    const std::vector<CacheEntry>& getEntries() const { return entries; }
    int getHitCount() const { return hitCount; }
    int getMissCount() const { return missCount; }
};

} // namespace mp
