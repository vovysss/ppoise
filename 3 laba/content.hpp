#pragma once

#include "audio.hpp"
#include "exceptions.hpp"
#include "library.hpp"
#include "playback.hpp"
#include "user.hpp"
#include "util.hpp"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace mp {

class Recommendation {
    Track track;
    double score = 0.0;
    std::string reason;
    std::string source;

public:
    Recommendation() = default;

    Recommendation(Track track, double score, std::string reason, std::string source)
        : track(std::move(track)),
          score(std::max(0.0, std::min(1.0, score))),
          reason(std::move(reason)),
          source(std::move(source)) {}

    bool isRelevant() const {
        return score >= 0.5;
    }

    std::string explain() const {
        return reason + " (" + source + ", score " + std::to_string(score) + ")";
    }

    void boost(double delta) {
        score = std::max(0.0, std::min(1.0, score + delta));
    }

    std::string describe() const {
        return track.getTitle() + " by " + track.getArtistName() + " - " + explain();
    }

    const Track& getTrack() const { return track; }
    double getScore() const { return score; }
    const std::string& getReason() const { return reason; }
    const std::string& getSource() const { return source; }
};

class RecommendationEngine {
    const MusicLibrary* library = nullptr;
    const PlaybackHistory* history = nullptr;
    std::map<std::string, double> genreWeights;
    double diversityFactor = 0.8;
    bool explicitAllowed = false;

public:
    RecommendationEngine() = default;

    RecommendationEngine(const MusicLibrary* library, const PlaybackHistory* history)
        : library(library), history(history) {}

    std::vector<Recommendation> recommendByGenre(const Genre& genre, int limit) {
        std::vector<Recommendation> result;
        if (library == nullptr || limit <= 0) {
            return result;
        }
        const auto weightIt = genreWeights.find(genre.getName());
        const double weight = (weightIt == genreWeights.end()) ? 0.0 : weightIt->second;
        for (const Track& track : library->getTracks()) {
            if (!explicitAllowed && track.isExplicitContent()) {
                continue;
            }
            if (track.getGenre().getName() != genre.getName()) {
                continue;
            }
            const double score = std::min(1.0, 0.4 + track.getRating() / 10.0 +
                                                  std::log1p(track.getPlayCount()) / 10.0 + weight);
            result.emplace_back(track, score, "matches genre " + genre.getName(), "genre");
        }
        std::sort(result.begin(), result.end(), [](const Recommendation& left, const Recommendation& right) {
            return left.getScore() > right.getScore();
        });
        if (static_cast<int>(result.size()) > limit) {
            result.resize(static_cast<std::size_t>(limit));
        }
        return result;
    }

    std::vector<Recommendation> recommendSimilar(const Track& seed, int limit) {
        std::vector<Recommendation> result;
        if (library == nullptr || limit <= 0) {
            return result;
        }
        const auto weightIt = genreWeights.find(seed.getGenre().getName());
        const double weight = (weightIt == genreWeights.end()) ? 0.0 : weightIt->second;
        for (const Track& track : library->getTracks()) {
            if (track.getId() == seed.getId()) {
                continue;
            }
            if (!explicitAllowed && track.isExplicitContent()) {
                continue;
            }
            const bool sameGenre = track.getGenre().getName() == seed.getGenre().getName();
            const double ratingSimilarity = 1.0 - std::abs(track.getRating() - seed.getRating()) / 5.0;
            const double score = std::min(1.0, (sameGenre ? 0.5 : 0.1) + ratingSimilarity * 0.3 + weight);
            result.emplace_back(track, score, "similar to " + seed.getTitle(), "similar");
        }
        std::sort(result.begin(), result.end(), [](const Recommendation& left, const Recommendation& right) {
            return left.getScore() > right.getScore();
        });
        if (static_cast<int>(result.size()) > limit) {
            result.resize(static_cast<std::size_t>(limit));
        }
        return result;
    }

    void updateWeights(const std::string& genre, double delta) {
        double& weight = genreWeights[genre];
        weight = std::max(-1.0, std::min(1.0, weight + delta));
    }

    void filterExplicit(std::vector<Recommendation>& recommendations) const {
        recommendations.erase(std::remove_if(recommendations.begin(), recommendations.end(),
                                             [](const Recommendation& recommendation) {
                                                 return recommendation.getTrack().isExplicitContent();
                                             }),
                              recommendations.end());
    }

    std::vector<Recommendation> diversify(const std::vector<Recommendation>& recommendations,
                                           int maxPerGenre) const {
        std::vector<Recommendation> result;
        if (maxPerGenre <= 0) {
            return result;
        }
        std::map<std::string, int> counts;
        for (const Recommendation& recommendation : recommendations) {
            const std::string& genre = recommendation.getTrack().getGenre().getName();
            if (counts[genre] < maxPerGenre) {
                result.push_back(recommendation);
                ++counts[genre];
            }
        }
        return result;
    }

    std::vector<Recommendation> topN(const std::vector<Recommendation>& recommendations,
                                     int count) const {
        std::vector<Recommendation> result = recommendations;
        std::sort(result.begin(), result.end(), [](const Recommendation& left, const Recommendation& right) {
            return left.getScore() > right.getScore();
        });
        if (count >= 0 && static_cast<int>(result.size()) > count) {
            result.resize(static_cast<std::size_t>(count));
        }
        return result;
    }

    const MusicLibrary* getLibrary() const { return library; }
    const PlaybackHistory* getHistory() const { return history; }
    const std::map<std::string, double>& getGenreWeights() const { return genreWeights; }
    double getDiversityFactor() const { return diversityFactor; }
    bool isExplicitAllowed() const { return explicitAllowed; }
};

class ListeningStatistics {
    long long totalMinutes = 0;
    int playedTracks = 0;
    std::string topGenre;
    std::string topArtist;
    int streakDays = 0;
    std::map<std::string, int> genreCounts;

public:
    ListeningStatistics() = default;

    long long record(const Track& track) {
        totalMinutes += std::max(1, track.getDurationSeconds() / 60);
        ++playedTracks;
        ++genreCounts[track.getGenre().getName()];
        topArtist = track.getArtistName();
        topGenre = favoriteGenre();
        return totalMinutes;
    }

    double averageDaily(int days) const {
        if (days <= 0) {
            throw MusicPlayerException("Day count must be positive");
        }
        return static_cast<double>(totalMinutes) / days;
    }

    void reset() {
        totalMinutes = 0;
        playedTracks = 0;
        topGenre.clear();
        topArtist.clear();
        streakDays = 0;
        genreCounts.clear();
    }

    std::string favoriteGenre() const {
        if (genreCounts.empty()) {
            return topGenre.empty() ? "unknown" : topGenre;
        }
        auto best = std::max_element(genreCounts.begin(), genreCounts.end(),
                                     [](const auto& left, const auto& right) {
                                         return left.second < right.second;
                                     });
        return best->first;
    }

    std::string summary() const {
        return "minutes: " + std::to_string(totalMinutes) +
               ", tracks: " + std::to_string(playedTracks) +
               ", top genre: " + favoriteGenre() + ", top artist: " + topArtist;
    }

    long long getTotalMinutes() const { return totalMinutes; }
    int getPlayedTracks() const { return playedTracks; }
    const std::string& getTopGenre() const { return topGenre; }
    const std::string& getTopArtist() const { return topArtist; }
    int getStreakDays() const { return streakDays; }
    const std::map<std::string, int>& getGenreCounts() const { return genreCounts; }
};

class ChartEntry {
    int position = 0;
    int previousPosition = 0;
    int weeksOnChart = 1;
    Track track;

public:
    ChartEntry() = default;

    ChartEntry(Track track, int position, int previousPosition, int weeksOnChart)
        : position(position),
          previousPosition(previousPosition),
          weeksOnChart(weeksOnChart),
          track(std::move(track)) {}

    void moveUp() {
        if (position > 1) {
            --position;
        }
    }

    void moveDown() {
        ++position;
    }

    bool isNew() const {
        return weeksOnChart <= 1;
    }

    std::string trend() const {
        if (previousPosition == 0) {
            return "new";
        }
        if (position < previousPosition) {
            return "up";
        }
        if (position > previousPosition) {
            return "down";
        }
        return "same";
    }

    void reposition(int newPosition) {
        if (newPosition <= 0) {
            throw MusicPlayerException("Chart position must be positive");
        }
        previousPosition = position;
        position = newPosition;
    }

    int getPosition() const { return position; }
    int getPreviousPosition() const { return previousPosition; }
    int getWeeksOnChart() const { return weeksOnChart; }
    const Track& getTrack() const { return track; }
};

class Chart {
    std::string name;
    std::string period;
    std::vector<ChartEntry> entries;
    std::time_t updatedAt = 0;

public:
    Chart() = default;

    Chart(std::string name, std::string period)
        : name(std::move(name)), period(std::move(period)), updatedAt(util::now()) {}

    void addEntry(const Track& track) {
        const bool exists = std::any_of(entries.begin(), entries.end(), [&](const ChartEntry& entry) {
            return entry.getTrack().getId() == track.getId();
        });
        if (exists) {
            throw DuplicateTrackException("Track " + std::to_string(track.getId()) +
                                          " is already in chart " + name);
        }
        const int position = static_cast<int>(entries.size()) + 1;
        entries.emplace_back(track, position, position, 1);
        updatedAt = util::now();
    }

    std::vector<ChartEntry> getTop(int count) const {
        std::vector<ChartEntry> result = entries;
        std::sort(result.begin(), result.end(), [](const ChartEntry& left, const ChartEntry& right) {
            return left.getPosition() < right.getPosition();
        });
        if (count <= 0) {
            result.clear();
        } else if (static_cast<int>(result.size()) > count) {
            result.resize(static_cast<std::size_t>(count));
        }
        return result;
    }

    void removeTrack(int trackId) {
        const auto it = std::find_if(entries.begin(), entries.end(), [&](const ChartEntry& entry) {
            return entry.getTrack().getId() == trackId;
        });
        if (it == entries.end()) {
            throw TrackNotFoundException("Track " + std::to_string(trackId) +
                                         " is not in chart " + name);
        }
        entries.erase(it);
        recalculatePositions();
    }

    void recalculatePositions() {
        std::sort(entries.begin(), entries.end(), [](const ChartEntry& left, const ChartEntry& right) {
            return left.getPosition() < right.getPosition();
        });
        for (std::size_t i = 0; i < entries.size(); ++i) {
            entries[i].reposition(static_cast<int>(i) + 1);
        }
        updatedAt = util::now();
    }

    int findTrackPosition(int trackId) const {
        for (const ChartEntry& entry : entries) {
            if (entry.getTrack().getId() == trackId) {
                return entry.getPosition();
            }
        }
        return -1;
    }

    void clear() {
        entries.clear();
        updatedAt = util::now();
    }

    const std::string& getName() const { return name; }
    const std::string& getPeriod() const { return period; }
    const std::vector<ChartEntry>& getEntries() const { return entries; }
    std::time_t getUpdatedAt() const { return updatedAt; }
};

class Ticket {
    std::string seat;
    double price = 0.0;
    bool purchased = false;
    std::string qrCode;
    bool used = false;

public:
    Ticket() = default;

    Ticket(std::string seat, double price) : seat(std::move(seat)), price(price) {}

    void validate() const {
        if (seat.empty()) {
            throw TicketException("Ticket seat is empty");
        }
        if (price < 0) {
            throw TicketException("Ticket price cannot be negative");
        }
        if (qrCode.empty()) {
            throw TicketException("Ticket has no QR code");
        }
    }

    void markPurchased() {
        if (seat.empty() || price < 0) {
            throw TicketException("Cannot sell ticket without a valid seat and price");
        }
        purchased = true;
        if (qrCode.empty()) {
            qrCode = "QR-" + seat + "-" + std::to_string(static_cast<int>(price));
        }
    }

    double refund() {
        if (!purchased) {
            throw TicketException("Ticket was not purchased");
        }
        if (used) {
            throw TicketException("Used ticket cannot be refunded");
        }
        purchased = false;
        return price;
    }

    void markUsed() {
        validate();
        if (!purchased || used) {
            throw TicketException("Ticket cannot be used twice");
        }
        used = true;
    }

    std::string describe() const {
        return "seat " + seat + ", price " + std::to_string(price) + ", " +
               (purchased ? "purchased" : "available") + (used ? ", used" : "");
    }

    const std::string& getSeat() const { return seat; }
    double getPrice() const { return price; }
    bool isPurchased() const { return purchased; }
    const std::string& getQrCode() const { return qrCode; }
    bool isUsed() const { return used; }
};

class Concert {
    std::string venue;
    std::string city;
    std::time_t date = 0;
    double ticketPrice = 0.0;
    Artist artist;
    int soldTickets = 0;

public:
    Concert() = default;

    Concert(std::string venue, std::string city, std::time_t date, double ticketPrice,
            Artist artist)
        : venue(std::move(venue)),
          city(std::move(city)),
          date(date),
          ticketPrice(ticketPrice),
          artist(std::move(artist)) {}

    bool isUpcoming(std::time_t currentTime) const {
        return date > currentTime;
    }

    int daysUntil(std::time_t currentTime) const {
        const long long difference = static_cast<long long>(date) -
                                     static_cast<long long>(currentTime);
        return difference <= 0 ? 0 : static_cast<int>(difference / 86400);
    }

    std::string buyTicket(Ticket& ticket) {
        if (!isUpcoming(util::now())) {
            throw TicketException("Concert has already started");
        }
        ticket.markPurchased();
        ++soldTickets;
        return ticket.getQrCode();
    }

    std::string describe() const {
        return artist.getName() + " live at " + venue + ", " + city + ", price " +
               std::to_string(ticketPrice);
    }

    bool isSoldOut(int capacity) const {
        return soldTickets >= capacity;
    }

    const std::string& getVenue() const { return venue; }
    const std::string& getCity() const { return city; }
    std::time_t getDate() const { return date; }
    double getTicketPrice() const { return ticketPrice; }
    const Artist& getArtist() const { return artist; }
    int getSoldTickets() const { return soldTickets; }
};

class RadioStation {
    std::string name;
    std::string streamUrl;
    Genre genre;
    int bitrateKbps = 128;
    bool isLive = true;

public:
    RadioStation() = default;

    RadioStation(std::string name, std::string streamUrl, Genre genre, int bitrateKbps,
                 bool isLive)
        : name(std::move(name)),
          streamUrl(std::move(streamUrl)),
          genre(std::move(genre)),
          bitrateKbps(bitrateKbps),
          isLive(isLive) {}

    std::string tuneIn() const {
        if (!isAvailable()) {
            throw RadioUnavailableException("Radio station " + name + " is unavailable");
        }
        return "tuning " + name + " at " + streamUrl;
    }

    std::string tuneOut() const {
        return "stopped " + name;
    }

    bool isAvailable() const {
        return !streamUrl.empty() && bitrateKbps > 0;
    }

    std::string describe() const {
        return name + " (" + genre.getName() + ", " + std::to_string(bitrateKbps) + " kbps" +
               (isLive ? ", live" : ", recorded") + ")";
    }

    const std::string& getName() const { return name; }
    const std::string& getStreamUrl() const { return streamUrl; }
    const Genre& getGenre() const { return genre; }
    int getBitrateKbps() const { return bitrateKbps; }
    bool isLiveStream() const { return isLive; }
};

class PodcastEpisode {
    int number = 0;
    std::string title;
    int durationSeconds = 0;
    std::time_t publishedAt = 0;
    AudioFile audioFile;
    bool listened = false;

public:
    PodcastEpisode() = default;

    PodcastEpisode(int number, std::string title, int durationSeconds, std::time_t publishedAt,
                   AudioFile audioFile)
        : number(number),
          title(std::move(title)),
          durationSeconds(durationSeconds),
          publishedAt(publishedAt),
          audioFile(std::move(audioFile)) {}

    int play() const {
        audioFile.validate();
        return durationSeconds;
    }

    void markListened() {
        listened = true;
    }

    double progressAfter(int seconds) const {
        if (durationSeconds <= 0) {
            return 0.0;
        }
        return std::min(100.0, 100.0 * seconds / durationSeconds);
    }

    std::string formattedDuration() const {
        return util::formatDuration(durationSeconds);
    }

    int getNumber() const { return number; }
    const std::string& getTitle() const { return title; }
    int getDurationSeconds() const { return durationSeconds; }
    std::time_t getPublishedAt() const { return publishedAt; }
    const AudioFile& getAudioFile() const { return audioFile; }
    bool isListened() const { return listened; }
};

class Podcast {
    std::string title;
    std::string host;
    std::string category;
    bool explicitContent = false;
    std::vector<PodcastEpisode> episodes;

public:
    Podcast() = default;

    Podcast(std::string title, std::string host, std::string category, bool explicitContent)
        : title(std::move(title)),
          host(std::move(host)),
          category(std::move(category)),
          explicitContent(explicitContent) {}

    void addEpisode(const PodcastEpisode& episode) {
        const bool exists = std::any_of(episodes.begin(), episodes.end(),
                                        [&](const PodcastEpisode& item) {
                                            return item.getNumber() == episode.getNumber();
                                        });
        if (exists) {
            throw MusicPlayerException("Episode " + std::to_string(episode.getNumber()) +
                                       " already exists in podcast " + title);
        }
        episodes.push_back(episode);
    }

    const PodcastEpisode* latestEpisode() const {
        if (episodes.empty()) {
            return nullptr;
        }
        const PodcastEpisode* latest = &episodes.front();
        for (const PodcastEpisode& episode : episodes) {
            if (episode.getNumber() > latest->getNumber()) {
                latest = &episode;
            }
        }
        return latest;
    }

    int totalDuration() const {
        int total = 0;
        for (const PodcastEpisode& episode : episodes) {
            total += episode.getDurationSeconds();
        }
        return total;
    }

    int countListened() const {
        return static_cast<int>(std::count_if(episodes.begin(), episodes.end(),
                                             [](const PodcastEpisode& episode) {
                                                 return episode.isListened();
                                             }));
    }

    bool isExplicit() const {
        return explicitContent;
    }

    const std::string& getTitle() const { return title; }
    const std::string& getHost() const { return host; }
    const std::string& getCategory() const { return category; }
    const std::vector<PodcastEpisode>& getEpisodes() const { return episodes; }
};

class Chapter {
    int number = 0;
    std::string title;
    int startSeconds = 0;
    int endSeconds = 0;

public:
    Chapter() = default;

    Chapter(int number, std::string title, int startSeconds, int endSeconds)
        : number(number),
          title(std::move(title)),
          startSeconds(startSeconds),
          endSeconds(endSeconds) {}

    bool contains(int seconds) const {
        return seconds >= startSeconds && seconds < endSeconds;
    }

    int duration() const {
        return std::max(0, endSeconds - startSeconds);
    }

    std::string describe() const {
        return "Chapter " + std::to_string(number) + ": " + title + " (" +
               util::formatDuration(duration()) + ")";
    }

    bool overlap(const Chapter& other) const {
        return startSeconds < other.endSeconds && other.startSeconds < endSeconds;
    }

    int getNumber() const { return number; }
    const std::string& getTitle() const { return title; }
    int getStartSeconds() const { return startSeconds; }
    int getEndSeconds() const { return endSeconds; }
};

class Audiobook {
    std::string title;
    std::string author;
    std::string narrator;
    std::vector<Chapter> chapters;
    int currentChapter = 1;
    int positionSeconds = 0;

public:
    Audiobook() = default;

    Audiobook(std::string title, std::string author, std::string narrator)
        : title(std::move(title)), author(std::move(author)), narrator(std::move(narrator)) {}

    void addChapter(const Chapter& chapter) {
        if (chapter.getStartSeconds() < 0 || chapter.getEndSeconds() <= chapter.getStartSeconds()) {
            throw MusicPlayerException("Chapter time range is invalid");
        }
        chapters.push_back(chapter);
        std::sort(chapters.begin(), chapters.end(), [](const Chapter& left, const Chapter& right) {
            return left.getStartSeconds() < right.getStartSeconds();
        });
    }

    void resumeAt(int seconds) {
        int total = 0;
        for (const Chapter& chapter : chapters) {
            total += chapter.duration();
        }
        if (seconds < 0 || seconds > total) {
            throw MusicPlayerException("Audiobook position is out of range");
        }
        positionSeconds = seconds;
        for (const Chapter& chapter : chapters) {
            if (chapter.contains(seconds)) {
                currentChapter = chapter.getNumber();
                break;
            }
        }
    }

    int remainingSeconds() const {
        int total = 0;
        for (const Chapter& chapter : chapters) {
            total += chapter.duration();
        }
        return std::max(0, total - positionSeconds);
    }

    const Chapter* chapterAt(int seconds) const {
        for (const Chapter& chapter : chapters) {
            if (chapter.contains(seconds)) {
                return &chapter;
            }
        }
        return nullptr;
    }

    double progressPercent() const {
        int total = 0;
        for (const Chapter& chapter : chapters) {
            total += chapter.duration();
        }
        if (total <= 0) {
            return 0.0;
        }
        return std::min(100.0, 100.0 * positionSeconds / total);
    }

    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    const std::string& getNarrator() const { return narrator; }
    const std::vector<Chapter>& getChapters() const { return chapters; }
    int getCurrentChapter() const { return currentChapter; }
    int getPositionSeconds() const { return positionSeconds; }
};

class SocialPost {
    int authorId = 0;
    std::string text;
    int likes = 0;
    std::time_t createdAt = 0;
    const Track* track = nullptr;
    std::vector<std::string> comments;

public:
    SocialPost() = default;

    SocialPost(int authorId, std::string text, const Track* track)
        : authorId(authorId),
          text(std::move(text)),
          createdAt(util::now()),
          track(track) {}

    void like() {
        ++likes;
    }

    void unlike() {
        if (likes > 0) {
            --likes;
        }
    }

    void addComment(const std::string& comment) {
        if (comment.empty()) {
            throw MusicPlayerException("Comment cannot be empty");
        }
        comments.push_back(comment);
    }

    bool isPopular() const {
        return likes >= 100;
    }

    std::string summary() const {
        return text + (track != nullptr ? " [" + track->getTitle() + "]" : "") +
               " - " + std::to_string(likes) + " likes, " +
               std::to_string(comments.size()) + " comments";
    }

    bool authoredBy(const User& user) const {
        return user.getId() == authorId;
    }

    int getAuthorId() const { return authorId; }
    const std::string& getText() const { return text; }
    int getLikes() const { return likes; }
    std::time_t getCreatedAt() const { return createdAt; }
    const Track* getTrack() const { return track; }
    const std::vector<std::string>& getComments() const { return comments; }
};

class Notification {
    std::string title;
    std::string message;
    std::string type;
    bool read = false;
    std::time_t createdAt = 0;

public:
    Notification() = default;

    Notification(std::string title, std::string message, std::string type)
        : title(std::move(title)),
          message(std::move(message)),
          type(std::move(type)),
          createdAt(util::now()) {}

    void markRead() {
        read = true;
    }

    bool isImportant() const {
        return type == "payment" || type == "subscription" || type == "security";
    }

    std::string summary() const {
        return "[" + type + "] " + title + ": " + message;
    }

    std::string ageDescription(std::time_t currentTime) const {
        const long long seconds = static_cast<long long>(currentTime) -
                                  static_cast<long long>(createdAt);
        if (seconds < 60) {
            return "just now";
        }
        if (seconds < 3600) {
            return std::to_string(seconds / 60) + " min ago";
        }
        if (seconds < 86400) {
            return std::to_string(seconds / 3600) + " h ago";
        }
        return std::to_string(seconds / 86400) + " d ago";
    }

    const std::string& getTitle() const { return title; }
    const std::string& getMessage() const { return message; }
    const std::string& getType() const { return type; }
    bool isRead() const { return read; }
    std::time_t getCreatedAt() const { return createdAt; }
};

class DeviceSync {
    std::string deviceName;
    std::time_t lastSyncAt = 0;
    std::vector<int> syncedTrackIds;
    bool enabled = true;

public:
    DeviceSync() = default;

    explicit DeviceSync(std::string deviceName) : deviceName(std::move(deviceName)) {}

    int sync(const std::vector<Track>& tracks) {
        if (!enabled) {
            throw DeviceNotConnectedException("Device " + deviceName + " is disabled for sync");
        }
        syncedTrackIds.clear();
        for (const Track& track : tracks) {
            syncedTrackIds.push_back(track.getId());
        }
        lastSyncAt = util::now();
        return static_cast<int>(syncedTrackIds.size());
    }

    void disconnect() {
        enabled = false;
    }

    bool needsSync(std::time_t currentTime, int hours) const {
        if (!enabled) {
            return false;
        }
        if (lastSyncAt == 0) {
            return true;
        }
        return currentTime - lastSyncAt > static_cast<long>(hours) * 3600L;
    }

    int syncedCount() const {
        return static_cast<int>(syncedTrackIds.size());
    }

    const std::string& getDeviceName() const { return deviceName; }
    std::time_t getLastSyncAt() const { return lastSyncAt; }
    const std::vector<int>& getSyncedTrackIds() const { return syncedTrackIds; }
    bool isEnabled() const { return enabled; }
};

class Scrobbler {
    std::string serviceName;
    bool enabled = true;
    std::vector<Track> pending;
    int submittedCount = 0;

public:
    Scrobbler() = default;

    explicit Scrobbler(std::string serviceName) : serviceName(std::move(serviceName)) {}

    bool scrobble(const Track& track) {
        if (!enabled) {
            return false;
        }
        pending.push_back(track);
        return true;
    }

    int flush() {
        const int count = static_cast<int>(pending.size());
        submittedCount += count;
        pending.clear();
        return count;
    }

    bool toggle() {
        enabled = !enabled;
        return enabled;
    }

    int pendingCount() const {
        return static_cast<int>(pending.size());
    }

    const std::string& getServiceName() const { return serviceName; }
    bool isEnabled() const { return enabled; }
    const std::vector<Track>& getPending() const { return pending; }
    int getSubmittedCount() const { return submittedCount; }
};

class DownloadTask {
    Track track;
    std::string status = "queued";
    int progressPercent = 0;
    std::time_t startedAt = 0;
    std::string errorMessage;
    double sizeMb = 0.0;

public:
    DownloadTask() = default;

    DownloadTask(Track track, double sizeMb)
        : track(std::move(track)), sizeMb(sizeMb) {}

    void start() {
        if (status == "completed") {
            throw DownloadException("Completed download cannot be restarted");
        }
        status = "downloading";
        startedAt = util::now();
    }

    void updateProgress(int percent) {
        if (status != "downloading") {
            throw DownloadException("Download is not running");
        }
        progressPercent = std::max(0, std::min(100, percent));
        if (progressPercent >= 100) {
            status = "completed";
        }
    }

    void complete() {
        progressPercent = 100;
        status = "completed";
    }

    void fail(const std::string& reason) {
        status = "failed";
        errorMessage = reason;
    }

    void cancel() {
        if (status == "completed") {
            throw DownloadException("Completed download cannot be canceled");
        }
        status = "canceled";
    }

    bool isDone() const {
        return status == "completed" || status == "failed" || status == "canceled";
    }

    const Track& getTrack() const { return track; }
    const std::string& getStatus() const { return status; }
    int getProgressPercent() const { return progressPercent; }
    std::time_t getStartedAt() const { return startedAt; }
    const std::string& getErrorMessage() const { return errorMessage; }
    double getSizeMb() const { return sizeMb; }
};

class DownloadManager {
    std::vector<DownloadTask> tasks;
    double storageLimitMb = 2048.0;
    double usedMb = 0.0;
    bool wifiOnly = true;

public:
    DownloadManager() = default;

    explicit DownloadManager(double storageLimitMb) : storageLimitMb(storageLimitMb) {}

    int queueDownload(const Track& track, double sizeMb) {
        if (sizeMb <= 0) {
            throw DownloadException("Download size must be positive");
        }
        if (usedMb + sizeMb > storageLimitMb) {
            throw StorageLimitException("Not enough storage for download");
        }
        const bool exists = std::any_of(tasks.begin(), tasks.end(), [&](const DownloadTask& task) {
            return task.getTrack().getId() == track.getId() && !task.isDone();
        });
        if (exists) {
            throw DownloadException("Track " + std::to_string(track.getId()) +
                                    " is already being downloaded");
        }
        tasks.emplace_back(track, sizeMb);
        return static_cast<int>(tasks.size()) - 1;
    }

    void cancel(int trackId) {
        for (DownloadTask& task : tasks) {
            if (task.getTrack().getId() == trackId) {
                task.cancel();
                return;
            }
        }
        throw DownloadException("Download for track " + std::to_string(trackId) + " not found");
    }

    int process() {
        int completed = 0;
        for (DownloadTask& task : tasks) {
            if (task.getStatus() == "queued" || task.getStatus() == "downloading") {
                if (usedMb + task.getSizeMb() > storageLimitMb) {
                    throw StorageLimitException("Storage limit reached during download");
                }
                task.complete();
                usedMb += task.getSizeMb();
                ++completed;
            }
        }
        return completed;
    }

    double totalSize() const {
        double total = 0.0;
        for (const DownloadTask& task : tasks) {
            total += task.getSizeMb();
        }
        return total;
    }

    int clearCompleted() {
        const std::size_t before = tasks.size();
        tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
                                   [](const DownloadTask& task) { return task.isDone(); }),
                    tasks.end());
        return static_cast<int>(before - tasks.size());
    }

    double freeSpace() const {
        return std::max(0.0, storageLimitMb - usedMb);
    }

    const std::vector<DownloadTask>& getTasks() const { return tasks; }
    double getStorageLimitMb() const { return storageLimitMb; }
    double getUsedMb() const { return usedMb; }
    bool isWifiOnly() const { return wifiOnly; }
};

} // namespace mp
