#include "musicplayer/music_player.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    using namespace mp;

    std::cout << "=== Music Player: demo of the object model ===\n";

    // --- Audio library and metadata -----------------------------------------
    Genre rock("Rock", "Guitar driven music", "#e63946");
    Genre indie("Indie Rock", "Independent rock scene", "#f4a261", "Rock");
    Genre jazz("Jazz", "Improvisation and swing", "#2a9d8f");
    Genre electronic("Electronic", "Synthesized sounds", "#457b9d");

    AudioFormat mp3("MP3", "mp3", false, 320);
    AudioFormat flac("FLAC", "flac", true, 1411);

    AudioFile fileA("/music/midnight_drive.mp3", 9LL * 1024 * 1024, mp3, 320, 44100);
    AudioFile fileB("/music/blue_hour.mp3", 7LL * 1024 * 1024, mp3, 256, 48000);
    AudioFile fileC("/music/neon_rain.flac", 32LL * 1024 * 1024, flac, 900, 96000);

    Track midnight(1, "Midnight Drive", "The Night Owls", 245, fileA, rock, 1);
    Track blueHour(2, "Blue Hour", "Jazz Avenue", 312, fileB, jazz, 1);
    Track neonRain(3, "Neon Rain", "Synth Garden", 198, fileC, electronic, 1, true);

    midnight.rate(5);
    blueHour.rate(4);
    neonRain.rate(3);
    midnight.play();
    midnight.play();
    blueHour.play();

    std::cout << "Track: " << midnight.getTitle() << " (" << midnight.formattedDuration()
              << ", " << midnight.getGenre().hierarchy() << ")\n";
    std::cout << "File: " << fileC.describe() << ", high quality: "
              << (fileC.isHighQuality() ? "yes" : "no") << "\n";
    std::cout << "Format: " << mp3.describe() << ", supports 900 kbps: "
              << (mp3.supportsBitrate(900) ? "yes" : "no") << "\n";

    Lyrics lyrics("en", "user", true);
    lyrics.addLine(LyricLine(0, "City lights are passing by"));
    lyrics.addLine(LyricLine(12000, "We are driving through the night", "Мы едем сквозь ночь"));
    std::cout << "Lyric at 12.2s: " << lyrics.textAt(12200)
              << ", translation coverage: " << lyrics.translationCoverage() << "%\n";

    AudioAnalyzer analyzer;
    analyzer.analyze(midnight);
    std::cout << "Analysis: " << analyzer.getBpm() << " BPM, key "
              << analyzer.getMusicalKey() << ", energy " << analyzer.energyLevel()
              << ", danceable: " << (analyzer.isDanceable() ? "yes" : "no") << "\n";

    Album album("Night Sessions", 2026, "Lunar Records", "/covers/night.jpg", rock);
    album.addTrack(midnight);
    album.addTrack(blueHour);
    std::cout << "Album " << album.getTitle() << ": " << album.getTracks().size()
              << " tracks, " << album.totalDuration() << " sec, average rating "
              << album.averageRating() << "\n";

    Artist artist("The Night Owls", "UK", rock, 250000);
    artist.verify();
    std::cout << "Artist: " << artist.displayName() << " (" << artist.rank() << ")\n";

    // --- Playback ------------------------------------------------------------
    Equalizer equalizer;
    equalizer.applyPreset("Bass Boost");
    Headphones headphones("Sony WH-1000XM5", "Bluetooth", 26.0);
    headphones.enableAnc();

    PlaybackQueue queue;
    queue.enqueue(midnight);
    queue.enqueue(blueHour);
    queue.enqueue(neonRain);
    queue.chooseRepeatMode("all");

    PlaybackHistory history(100);
    Player player(queue, equalizer, &headphones, &history);
    player.changeVolume(70);

    const Track& nowPlaying = player.play();
    std::cout << "Playing: " << nowPlaying.getTitle() << " via " << headphones.status() << "\n";
    player.listenFor(40);
    player.pause();
    player.resume();
    player.stop();
    std::cout << "History entries: " << history.getSessions().size()
              << ", most played track id: " << history.mostPlayedTrackId()
              << ", total listening: " << history.getTotalListeningSeconds() << " sec\n";
    std::cout << "Equalizer preset: " << player.getEqualizer().getPresetName()
              << ", average gain: " << player.getEqualizer().averageGain() << " dB\n";

    SleepTimer sleepTimer;
    sleepTimer.start(30);
    sleepTimer.tick(300);
    std::cout << "Sleep timer remaining: " << sleepTimer.remainingFormatted()
              << ", active: " << (sleepTimer.isActive() ? "yes" : "no") << "\n";

    // --- Library, playlists and search --------------------------------------
    MusicLibrary library("/music");
    library.addTrack(midnight);
    library.addTrack(blueHour);
    library.addTrack(neonRain);
    library.addAlbum(album);
    library.addArtist(artist);

    Playlist playlist("Late Night", "Tracks for the road", true);
    playlist.addTrack(midnight, "vlad");
    playlist.addTrack(blueHour, "vlad");
    playlist.moveTrack(2, 1);
    playlist.markPlayed();
    library.addPlaylist(playlist);
    std::cout << "Playlist " << playlist.getName() << ": " << playlist.getEntries().size()
              << " tracks, link " << playlist.shareLink() << "\n";

    SearchQuery query("night", "any", false, 10, Genre());
    SearchEngine searchEngine(&library);
    const auto found = searchEngine.searchTracks(query);
    std::cout << "Search '" << query.getText() << "': " << found.size()
              << " result(s), library stats: " << library.statistics() << "\n";

    MediaScanner scanner;
    const int scanned = scanner.scanDirectory("/music", {"/music/a.mp3", "/music/b.flac",
                                                         "/music/cover.jpg"});
    std::cout << "Scanner: " << scanned << " files, supported formats: "
              << scanner.getSupportedExtensions().size() << "\n";

    CacheManager cache(64.0);
    cache.cacheTrack(midnight, 8.5);
    cache.cacheTrack(blueHour, 7.0);
    std::cout << "Cache hit: " << (cache.isCached(midnight.getId()) ? "yes" : "no")
              << ", used " << cache.getCurrentSizeMb() << " MB, hit rate "
              << cache.hitRate() << "%\n";

    // --- User, subscription, payments --------------------------------------
    UserProfile profile("Vladimir", rock);
    profile.changeAvatar("/avatars/vlad.png");
    profile.updateBio("Music lover and C++ student");
    profile.follow();
    User user(1, "vlad", "vlad@example.com", "password123", profile);
    user.authenticate("password123");

    SubscriptionPlan freePlan("Free", 0.0, 1, false, false, false);
    SubscriptionPlan premiumPlan("Premium", 9.99, 5, true, true, true);
    user.upgrade(premiumPlan);
    user.requirePremium();
    std::cout << "User " << user.displayName() << ": premium="
              << (user.isPremium() ? "yes" : "no") << ", plan "
              << user.getSubscription().getPlan().describe()
              << ", days left " << user.getSubscription().daysLeft() << "\n";

    PaymentMethod card("4242", "Visa", 12, 2030, "Vladimir");
    Payment payment(101, 9.99, "USD", card);
    payment.process();
    std::cout << "Payment " << payment.getId() << ": " << payment.formattedAmount()
              << " -> " << payment.getStatus() << "\n";

    Invoice invoice("INV-2026-11");
    invoice.addItem(InvoiceItem("Premium subscription", 1, 9.99));
    invoice.addItem(InvoiceItem("Hi-Fi add-on", 1, 2.99));
    PromoCode promo("MUSIC10", 10, util::now() + 86400, 100);
    invoice.applyPromo(promo);
    invoice.markPaid();
    std::cout << "Invoice " << invoice.getNumber() << ": total "
              << invoice.total() << " USD, paid=" << (invoice.isPaid() ? "yes" : "no")
              << ", promo " << promo.describe() << "\n";

    // --- Discovery and statistics ------------------------------------------
    RecommendationEngine recommender(&library, &history);
    recommender.updateWeights("Rock", 0.1);
    const auto recommendations = recommender.recommendByGenre(rock, 5);
    std::cout << "Recommendations for Rock: " << recommendations.size() << "\n";
    for (const Recommendation& recommendation : recommendations) {
        std::cout << "  - " << recommendation.describe() << "\n";
    }

    ListeningStatistics statistics;
    statistics.record(midnight);
    statistics.record(blueHour);
    statistics.record(midnight);
    std::cout << "Statistics: " << statistics.summary() << ", daily average (7 days): "
              << statistics.averageDaily(7) << " min\n";

    Chart chart("Weekly Top", "2026-W46");
    chart.addEntry(midnight);
    chart.addEntry(blueHour);
    chart.addEntry(neonRain);
    chart.recalculatePositions();
    std::cout << "Chart " << chart.getName() << ": " << chart.getTop(2).size()
              << " top entries, midnight position " << chart.findTrackPosition(midnight.getId())
              << "\n";

    Concert concert("O2 Arena", "London", util::now() + 30 * 86400, 55.0, artist);
    Ticket ticket("A-12", 55.0);
    const std::string qr = concert.buyTicket(ticket);
    ticket.markUsed();
    std::cout << "Concert: " << concert.describe() << ", days until "
              << concert.daysUntil(util::now()) << ", ticket " << ticket.describe()
              << ", qr " << qr << "\n";

    RadioStation radio("Rock FM", "https://stream.example/rock", rock, 320, true);
    std::cout << "Radio: " << radio.describe() << " -> " << radio.tuneIn() << "\n";

    // --- Podcasts, audiobooks and social features --------------------------
    PodcastEpisode episode(1, "The History of Rock", 1800, util::now() - 3600, fileA);
    Podcast podcast("Sound Stories", "Anna", "music", false);
    podcast.addEpisode(episode);
    std::cout << "Podcast " << podcast.getTitle() << ": " << podcast.totalDuration()
              << " sec, latest: " << podcast.latestEpisode()->getTitle() << "\n";

    Audiobook audiobook("The Sound of Waves", "Yukio Mishima", "John Doe");
    audiobook.addChapter(Chapter(1, "Arrival", 0, 600));
    audiobook.addChapter(Chapter(2, "The Lighthouse", 600, 1200));
    audiobook.resumeAt(700);
    std::cout << "Audiobook: chapter " << audiobook.getCurrentChapter() << ", remaining "
              << audiobook.remainingSeconds() << " sec, progress "
              << audiobook.progressPercent() << "%\n";

    SocialPost post(user.getId(), "Midnight Drive is a masterpiece", &midnight);
    post.like();
    post.addComment("Agree!");
    std::cout << "Post: " << post.summary() << ", authored by user: "
              << (post.authoredBy(user) ? "yes" : "no") << "\n";

    Notification notification("Payment received", "Premium renewed", "payment");
    notification.markRead();
    std::cout << "Notification: " << notification.summary() << ", important: "
              << (notification.isImportant() ? "yes" : "no") << ", age: "
              << notification.ageDescription(util::now()) << "\n";

    // --- Services ------------------------------------------------------------
    DeviceSync deviceSync("iPhone 15");
    const int synced = deviceSync.sync(library.getTracks());
    std::cout << "Device sync: " << synced << " tracks, needs sync: "
              << (deviceSync.needsSync(util::now(), 1) ? "yes" : "no") << "\n";

    Scrobbler scrobbler("Last.fm");
    scrobbler.scrobble(midnight);
    scrobbler.scrobble(blueHour);
    std::cout << "Scrobbler: flushed " << scrobbler.flush() << " tracks, total submitted "
              << scrobbler.getSubmittedCount() << "\n";

    DownloadManager downloads(128.0);
    downloads.queueDownload(midnight, 8.5);
    downloads.queueDownload(blueHour, 7.0);
    const int completedDownloads = downloads.process();
    std::cout << "Downloads: " << completedDownloads << " completed, used "
              << downloads.getUsedMb() << " MB, free " << downloads.freeSpace() << " MB\n";

    // --- Exception demonstration -------------------------------------------
    try {
        player.changeVolume(150);
    } catch (const InvalidVolumeException& error) {
        std::cout << "Expected exception [" << error.code() << "]: " << error.what() << "\n";
    }

    try {
        Playlist limited("Limit", "test", false);
        limited.addTrack(midnight, "vlad");
        limited.addTrack(midnight, "vlad");
    } catch (const DuplicateTrackException& error) {
        std::cout << "Expected exception [" << error.code() << "]: " << error.what() << "\n";
    }

    std::cout << "=== Demo finished ===\n";
    return 0;
}
