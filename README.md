# Лабораторная работа №3. Объектная модель предметной области «Музыкальный проигрыватель»

## 1. О проекте

Учебный C++17-проект моделирует приложение-музыкальный проигрыватель: аудиотеку и метаданные, воспроизведение и звук, библиотеку, поиск, кэш, пользователей, подписки и платежи, рекомендации, контент, социальные функции и сервисы синхронизации.

Домен: **музыкальный проигрыватель и его экосистема**. Модель демонстрирует наследование, композицию, агрегацию, ассоциации, инкапсуляцию и собственные исключения. Все методы реализованы содержательно, а основные сценарии проверяются в `src/main.cpp`.

### Сборка и запуск

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp -o music_player
./music_player
```

Альтернативно:

```bash
cmake -S . -B build
cmake --build build
./build/music_player
```

### Правила подсчёта

- **Поля** — собственные поля класса; унаследованные поля не дублируются.
- **Поведения** — содержательные методы предметной области; геттеры и сеттеры (`get*`, `set*`) вынесены в раздел «Доступ» и не учитываются.
- Предикаты `is*`, `has*`, `can*` считаются поведениями, если выражают правило домена (например, `isPremium`, `canAddDevice`, `isDanceable`).
- **Ассоциации** — случаи, когда класс хранит другой доменный класс в поле или принимает его в параметре метода/конструктора.
- Исключения учитываются отдельно от доменных классов.

## 2. Итоговая статистика

| Показатель | Значение |
|---|---:|
| Доменные классы | 56 |
| Поля | 275 |
| Поведения (методы) | 322 |
| Ассоциации | 62 |
| Собственные исключения | 19 |

322 поведения — это 249 уникальных имён методов; одноимённые методы (`describe`, `clear`, `matches` и т. п.) имеют разную реализацию в разных классах.

Все количественные требования выполнены с запасом: не менее 50 классов, 150 полей, 100 поведений, 30 ассоциаций и 12 исключений.

## 3. Сводная таблица классов

Формат: `Класс | число полей | число поведений | связанные классы`.

В таблицу включены 56 доменных классов. Ещё 19 классов исключений описаны отдельно в разделе 5 и не входят в подсчёт доменных классов.

| Класс | Поля | Методы | Ассоциации (связанные классы) |
|---|---:|---:|---|
| Album | 6 | 6 | Genre, Track |
| Artist | 5 | 6 | Genre |
| AudioAnalyzer | 4 | 5 | Track |
| AudioDevice | 6 | 7 | — |
| AudioFile | 5 | 5 | AudioFormat |
| AudioFormat | 4 | 4 | — |
| Audiobook | 6 | 5 | Chapter |
| CacheEntry | 4 | 4 | — |
| CacheManager | 5 | 6 | CacheEntry, Track |
| Chapter | 4 | 4 | — |
| Chart | 4 | 5 | ChartEntry, Track |
| ChartEntry | 4 | 5 | Track |
| Concert | 6 | 5 | Artist, Ticket |
| DeviceSync | 4 | 5 | Track |
| DownloadManager | 4 | 7 | DownloadTask, Track |
| DownloadTask | 6 | 6 | Track |
| Equalizer | 3 | 6 | EqualizerBand |
| EqualizerBand | 4 | 4 | — |
| Genre | 4 | 4 | — |
| Headphones | 2 | 5 | — |
| Invoice | 6 | 6 | InvoiceItem, PromoCode |
| InvoiceItem | 3 | 3 | — |
| ListeningStatistics | 6 | 5 | Track |
| LyricLine | 3 | 4 | — |
| Lyrics | 4 | 6 | LyricLine |
| MediaScanner | 4 | 5 | — |
| MusicLibrary | 6 | 9 | Album, Artist, Playlist, Track |
| Notification | 5 | 5 | — |
| Payment | 7 | 5 | PaymentMethod |
| PaymentMethod | 5 | 3 | — |
| PlaybackHistory | 3 | 5 | PlaybackSession |
| PlaybackQueue | 4 | 10 | Track |
| PlaybackSession | 6 | 7 | Track |
| Player | 10 | 14 | AudioDevice, Equalizer, PlaybackHistory, PlaybackQueue |
| Playlist | 6 | 10 | PlaylistEntry, Track |
| PlaylistEntry | 4 | 4 | Track |
| Podcast | 5 | 5 | PodcastEpisode |
| PodcastEpisode | 6 | 5 | AudioFile |
| PromoCode | 5 | 5 | — |
| RadioStation | 5 | 5 | Genre |
| Recommendation | 4 | 4 | Track |
| RecommendationEngine | 5 | 7 | Genre, MusicLibrary, PlaybackHistory, Recommendation, Track |
| Scrobbler | 4 | 5 | Track |
| SearchEngine | 3 | 5 | MusicLibrary, SearchQuery, Track |
| SearchQuery | 5 | 5 | Genre, Track |
| SleepTimer | 4 | 7 | — |
| SocialPost | 6 | 6 | Track, User |
| Speaker | 2 | 3 | — |
| StreamQuality | 3 | 3 | — |
| Subscription | 5 | 8 | SubscriptionPlan |
| SubscriptionPlan | 6 | 7 | — |
| Ticket | 5 | 7 | — |
| Track | 10 | 8 | AudioFile, Genre |
| User | 9 | 9 | Subscription, SubscriptionPlan, UserProfile, UserSettings |
| UserProfile | 5 | 5 | Genre |
| UserSettings | 6 | 8 | — |

## 4. Подробное описание классов

В каждом разделе перечислены назначение класса, поля, поведения, вспомогательные методы доступа и ассоциации.

### 1. Аудиотека и метаданные

**AudioFormat**

Назначение: Формат аудио и его ограничения по битрейту.

Наследование: нет.

Поля (4): `name: std::string`, `extension: std::string`, `lossless: bool`, `maxBitrateKbps: int`.

Поведения (4): `bool supportsBitrate(int) const`, `bool isLossless() const`, `std::string describe() const`, `std::string buildFileName(const std::string &) const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getName() const`, `const std::string & getExtension() const`, `int getMaxBitrateKbps() const`.

Ассоциации (0): нет.

**AudioFile**

Назначение: Физический аудиофайл: путь, размер, формат, битрейт и частота дискретизации.

Наследование: нет.

Поля (5): `path: std::string`, `sizeBytes: long long`, `format: AudioFormat`, `bitrateKbps: int`, `sampleRateHz: int`.

Поведения (5): `void validate() const`, `double sizeInMb() const`, `bool isHighQuality() const`, `AudioFile convertTo(const AudioFormat &, int) const`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getPath() const`, `long long getSizeBytes() const`, `const AudioFormat & getFormat() const`, `int getBitrateKbps() const`, `int getSampleRateHz() const`.

Ассоциации (1): AudioFormat.

**LyricLine**

Назначение: Строка текста песни с таймкодом и переводом.

Наследование: нет.

Поля (3): `timeMs: long long`, `content: std::string`, `translation: std::string`.

Поведения (4): `bool matches(long long, int) const`, `void translate(const std::string &)`, `std::string display(bool) const`, `bool isEmpty() const`.

Доступ (геттеры/сеттеры, не учитываются): `long long getTimeMs() const`, `const std::string & getContent() const`, `const std::string & getTranslation() const`.

Ассоциации (0): нет.

**Lyrics**

Назначение: Текст песни с синхронизацией и переводом.

Наследование: нет.

Поля (4): `lines: std::vector<LyricLine>`, `language: std::string`, `source: std::string`, `synced: bool`.

Поведения (6): `void addLine(const LyricLine &)`, `std::string textAt(long long) const`, `bool hasTranslation() const`, `double translationCoverage() const`, `void clear()`, `bool isSynced() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::vector<LyricLine> & getLines() const`, `const std::string & getLanguage() const`, `const std::string & getSource() const`.

Ассоциации (1): LyricLine.

**Genre**

Назначение: Музыкальный жанр с иерархией и цветовой меткой.

Наследование: нет.

Поля (4): `name: std::string`, `description: std::string`, `colorHex: std::string`, `parentName: std::string`.

Поведения (4): `bool isSubgenreOf(const Genre &) const`, `bool matches(const std::string &) const`, `std::string badge() const`, `std::string hierarchy() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getName() const`, `const std::string & getDescription() const`, `const std::string & getColorHex() const`, `const std::string & getParentName() const`.

Ассоциации (0): нет.

**Track**

Назначение: Трек с метаданными, аудиофайлом, жанром, рейтингом и счётчиком проигрываний.

Наследование: нет.

Поля (10): `id: int`, `title: std::string`, `artistName: std::string`, `durationSeconds: int`, `trackNumber: int`, `file: AudioFile`, `genre: Genre`, `playCount: int`, `rating: int`, `explicitContent: bool`.

Поведения (8): `void validate() const`, `int play()`, `void rate(int)`, `bool isLong() const`, `std::string formattedDuration() const`, `bool matches(const std::string &) const`, `void mergeMetadata(const Track &)`, `bool isExplicitContent() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getId() const`, `const std::string & getTitle() const`, `const std::string & getArtistName() const`, `int getDurationSeconds() const`, `int getTrackNumber() const`, `const AudioFile & getFile() const`, `const Genre & getGenre() const`, `int getPlayCount() const`, `int getRating() const`.

Ассоциации (2): AudioFile, Genre.

**Album**

Назначение: Альбом как совокупность треков с жанром, лейблом и обложкой.

Наследование: нет.

Поля (6): `title: std::string`, `releaseYear: int`, `label: std::string`, `coverPath: std::string`, `genre: Genre`, `tracks: std::vector<Track>`.

Поведения (6): `void addTrack(const Track &)`, `void removeTrack(int)`, `int totalDuration() const`, `double averageRating() const`, `bool isComplete(int) const`, `const Track * findTrack(const std::string &) const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getTitle() const`, `int getReleaseYear() const`, `const std::string & getLabel() const`, `const std::string & getCoverPath() const`, `const Genre & getGenre() const`, `const std::vector<Track> & getTracks() const`.

Ассоциации (2): Genre, Track.

**Artist**

Назначение: Исполнитель, его страна, жанр, аудитория и статус верификации.

Наследование: нет.

Поля (5): `name: std::string`, `country: std::string`, `genre: Genre`, `monthlyListeners: long long`, `verified: bool`.

Поведения (6): `bool verify()`, `void addMonthlyListeners(long long)`, `bool matches(const std::string &) const`, `std::string displayName() const`, `std::string rank() const`, `bool isVerified() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getName() const`, `const std::string & getCountry() const`, `const Genre & getGenre() const`, `long long getMonthlyListeners() const`.

Ассоциации (1): Genre.

**AudioAnalyzer**

Назначение: Анализ звука: BPM, тональность, энергия и громкость.

Наследование: нет.

Поля (4): `bpm: int`, `musicalKey: std::string`, `energy: double`, `loudnessDb: double`.

Поведения (5): `void analyze(const Track &)`, `int detectBpm(const std::vector<int> &)`, `std::string detectKey(const std::vector<int> &)`, `std::string energyLevel() const`, `bool isDanceable() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getBpm() const`, `const std::string & getMusicalKey() const`, `double getEnergy() const`, `double getLoudnessDb() const`.

Ассоциации (1): Track.

### 2. Воспроизведение и звук

**EqualizerBand**

Назначение: Одна полоса эквалайзера с частотой и усилением.

Наследование: нет.

Поля (4): `frequencyHz: int`, `gainDb: double`, `minGainDb: double`, `maxGainDb: double`.

Поведения (4): `void adjustGain(double)`, `void reset()`, `bool isBoosted() const`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getFrequencyHz() const`, `double getGainDb() const`, `double getMinGainDb() const`, `double getMaxGainDb() const`.

Ассоциации (0): нет.

**Equalizer**

Назначение: Эквалайзер с набором полос и пресетами.

Наследование: нет.

Поля (3): `bands: std::vector<EqualizerBand>`, `presetName: std::string`, `enabled: bool`.

Поведения (6): `void adjustBand(std::size_t, double)`, `void applyPreset(const std::string &)`, `void reset()`, `bool toggle()`, `double averageGain() const`, `bool isEnabled() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::vector<EqualizerBand> & getBands() const`, `const std::string & getPresetName() const`.

Ассоциации (1): EqualizerBand.

**StreamQuality**

Назначение: Параметры качества потока: кодек, битрейт и частота.

Наследование: нет.

Поля (3): `bitrateKbps: int`, `codec: std::string`, `sampleRateHz: int`.

Поведения (3): `bool isLossless() const`, `void upgrade(int)`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getBitrateKbps() const`, `const std::string & getCodec() const`, `int getSampleRateHz() const`.

Ассоциации (0): нет.

**AudioDevice**

Назначение: Базовое аудиоустройство вывода с громкостью, задержкой и подключением.

Наследование: нет.

Поля (6): `name: std::string`, `type: std::string`, `connected: bool`, `volumeLevel: int`, `latencyMs: int`, `batteryPercent: int`.

Поведения (7): `void connect()`, `void disconnect()`, `void changeVolume(int)`, `bool testLatency() const`, `bool isWireless() const`, `std::string status() const`, `bool isConnected() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getName() const`, `const std::string & getType() const`, `int getVolumeLevel() const`, `int getLatencyMs() const`, `int getBatteryPercent() const`.

Ассоциации (0): нет.

**Headphones**

Назначение: Наушники с шумоподавлением и зарядом батареи.

Наследование: AudioDevice.

Поля (2): `noiseCancellation: bool`, `batteryHours: double`.

Поведения (5): `void enableAnc()`, `void disableAnc()`, `void charge(int)`, `std::string ancStatus() const`, `bool isNoiseCancellation() const`.

Доступ (геттеры/сеттеры, не учитываются): `double getBatteryHours() const`.

Ассоциации (0): нет.

**Speaker**

Назначение: Колонка с числом каналов и мощностью.

Наследование: AudioDevice.

Поля (2): `channels: int`, `wattage: int`.

Поведения (3): `void chooseStereo(bool)`, `int powerRating() const`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getChannels() const`, `int getWattage() const`.

Ассоциации (0): нет.

**PlaybackQueue**

Назначение: Очередь воспроизведения с повтором и перемешиванием.

Наследование: нет.

Поля (4): `tracks: std::vector<Track>`, `currentIndex: int`, `shuffleEnabled: bool`, `repeatMode: std::string`.

Поведения (10): `void enqueue(const Track &)`, `void removeAt(std::size_t)`, `const Track * next()`, `const Track * previous()`, `bool toggleShuffle()`, `void chooseRepeatMode(const std::string &)`, `void clear()`, `const Track * current() const`, `bool isShuffleEnabled() const`, `bool isEmpty() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::vector<Track> & getTracks() const`, `int getCurrentIndex() const`, `const std::string & getRepeatMode() const`.

Ассоциации (1): Track.

**PlaybackSession**

Назначение: Сессия прослушивания одного трека с прогрессом.

Наследование: нет.

Поля (6): `track: Track`, `startedAt: std::time_t`, `listenedSeconds: int`, `completed: bool`, `deviceName: std::string`, `paused: bool`.

Поведения (7): `void pause()`, `void resume()`, `void finish(int)`, `double completionPercent() const`, `bool isCompleted() const`, `std::string summary() const`, `bool isPaused() const`.

Доступ (геттеры/сеттеры, не учитываются): `const Track & getTrack() const`, `std::time_t getStartedAt() const`, `int getListenedSeconds() const`, `const std::string & getDeviceName() const`.

Ассоциации (1): Track.

**PlaybackHistory**

Назначение: История сессий прослушивания и подсчёт проигрываний.

Наследование: нет.

Поля (3): `sessions: std::vector<PlaybackSession>`, `maxEntries: std::size_t`, `totalListeningSeconds: long long`.

Поведения (5): `void record(const PlaybackSession &)`, `std::vector<PlaybackSession> recent(int) const`, `int countPlays(int) const`, `int mostPlayedTrackId() const`, `void clear()`.

Доступ (геттеры/сеттеры, не учитываются): `const std::vector<PlaybackSession> & getSessions() const`, `std::size_t getMaxEntries() const`, `long long getTotalListeningSeconds() const`.

Ассоциации (1): PlaybackSession.

**Player**

Назначение: Плеер: управление очередью, громкостью, скоростью, эквалайзером и устройством.

Наследование: нет.

Поля (10): `queue: PlaybackQueue`, `state: std::string`, `volume: int`, `positionSeconds: int`, `playbackSpeed: double`, `equalizer: Equalizer`, `device: AudioDevice *`, `history: PlaybackHistory *`, `muted: bool`, `crossfadeSeconds: int`.

Поведения (14): `const Track & play()`, `void pause()`, `void resume()`, `void stop()`, `const Track * next()`, `const Track * previous()`, `void seek(int)`, `void changeVolume(int)`, `bool toggleMute()`, `void changeSpeed(double)`, `void applyEqualizer(const std::string &)`, `void listenFor(int)`, `std::string currentTitle() const`, `bool isMuted() const`.

Доступ (геттеры/сеттеры, не учитываются): `const PlaybackQueue & getQueue() const`, `const std::string & getState() const`, `int getVolume() const`, `int getPositionSeconds() const`, `double getPlaybackSpeed() const`, `const Equalizer & getEqualizer() const`, `int getCrossfadeSeconds() const`, `void setHistory(PlaybackHistory *)`, `void setDevice(AudioDevice *)`.

Ассоциации (4): AudioDevice, Equalizer, PlaybackHistory, PlaybackQueue.

**SleepTimer**

Назначение: Таймер сна с обратным отсчётом и затуханием звука.

Наследование: нет.

Поля (4): `durationMinutes: int`, `remainingSeconds: int`, `active: bool`, `fadeOut: bool`.

Поведения (7): `void start(int)`, `void tick(int)`, `void cancel()`, `bool isExpired() const`, `std::string remainingFormatted() const`, `bool isActive() const`, `bool isFadeOut() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getDurationMinutes() const`, `int getRemainingSeconds() const`, `void setFadeOut(bool)`.

Ассоциации (0): нет.

### 3. Библиотека, поиск и кэш

**PlaylistEntry**

Назначение: Позиция трека в плейлисте с автором добавления.

Наследование: нет.

Поля (4): `track: Track`, `position: int`, `addedAt: std::time_t`, `addedBy: std::string`.

Поведения (4): `void moveUp()`, `void moveDown()`, `void updatePosition(int)`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `const Track & getTrack() const`, `int getPosition() const`, `std::time_t getAddedAt() const`, `const std::string & getAddedBy() const`.

Ассоциации (1): Track.

**Playlist**

Назначение: Плейлист с упорядоченными треками, перемещением и публичной ссылкой.

Наследование: нет.

Поля (6): `name: std::string`, `description: std::string`, `isPublic: bool`, `createdAt: std::time_t`, `entries: std::vector<PlaylistEntry>`, `playCount: int`.

Поведения (10): `void addTrack(const Track &, const std::string &)`, `void removeTrack(int)`, `void moveTrack(int, int)`, `int totalDuration() const`, `bool contains(int) const`, `void shuffle()`, `void clear()`, `std::string shareLink() const`, `void markPlayed()`, `bool isPublicPlaylist() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getName() const`, `const std::string & getDescription() const`, `std::time_t getCreatedAt() const`, `const std::vector<PlaylistEntry> & getEntries() const`, `int getPlayCount() const`.

Ассоциации (2): PlaylistEntry, Track.

**MusicLibrary**

Назначение: Основная библиотека треков, альбомов, исполнителей и плейлистов.

Наследование: нет.

Поля (6): `tracks: std::vector<Track>`, `albums: std::vector<Album>`, `artists: std::vector<Artist>`, `playlists: std::vector<Playlist>`, `lastScanAt: std::time_t`, `storagePath: std::string`.

Поведения (9): `void addTrack(const Track &)`, `void removeTrack(int)`, `void addAlbum(const Album &)`, `void addArtist(const Artist &)`, `void addPlaylist(const Playlist &)`, `const Track * findTrackById(int) const`, `std::vector<Track> searchByTitle(const std::string &) const`, `std::string statistics() const`, `void clear()`.

Доступ (геттеры/сеттеры, не учитываются): `const std::vector<Track> & getTracks() const`, `const std::vector<Album> & getAlbums() const`, `const std::vector<Artist> & getArtists() const`, `const std::vector<Playlist> & getPlaylists() const`, `std::time_t getLastScanAt() const`, `const std::string & getStoragePath() const`.

Ассоциации (4): Album, Artist, Playlist, Track.

**SearchQuery**

Назначение: Параметры поискового запроса и правило сопоставления с треком.

Наследование: нет.

Поля (5): `text: std::string`, `field: std::string`, `caseSensitive: bool`, `limit: int`, `genre: Genre`.

Поведения (5): `std::string normalize() const`, `bool isValid() const`, `bool matches(const Track &) const`, `std::string describe() const`, `bool isCaseSensitive() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getText() const`, `const std::string & getField() const`, `int getLimit() const`, `const Genre & getGenre() const`.

Ассоциации (2): Genre, Track.

**SearchEngine**

Назначение: Поиск по библиотеке с учётом полей, лимита и релевантности.

Наследование: нет.

Поля (3): `library: const MusicLibrary *`, `history: std::vector<std::string>`, `maxResults: int`.

Поведения (5): `std::vector<Track> searchTracks(const SearchQuery &)`, `std::vector<Playlist> searchPlaylists(const std::string &)`, `void sortByRelevance(std::vector<Track> &) const`, `std::vector<Track> searchByArtist(const std::string &)`, `void clearHistory()`.

Доступ (геттеры/сеттеры, не учитываются): `const MusicLibrary * getLibrary() const`, `const std::vector<std::string> & getHistory() const`, `int getMaxResults() const`.

Ассоциации (3): MusicLibrary, SearchQuery, Track.

**MediaScanner**

Назначение: Сканирование каталогов и распознавание поддерживаемых форматов.

Наследование: нет.

Поля (4): `supportedExtensions: std::vector<std::string>`, `foundCount: int`, `lastScanPath: std::string`, `ignoredPaths: std::vector<std::string>`.

Поведения (5): `void registerFormat(const std::string &)`, `int scanDirectory(const std::string &, const std::vector<std::string> &)`, `bool isSupported(const std::string &) const`, `std::string report() const`, `void ignorePath(const std::string &)`.

Доступ (геттеры/сеттеры, не учитываются): `const std::vector<std::string> & getSupportedExtensions() const`, `int getFoundCount() const`, `const std::string & getLastScanPath() const`, `const std::vector<std::string> & getIgnoredPaths() const`.

Ассоциации (0): нет.

**CacheEntry**

Назначение: Запись кэша одного трека с размером, временем доступа и числом попаданий.

Наследование: нет.

Поля (4): `trackId: int`, `sizeMb: double`, `lastAccessedAt: std::time_t`, `hits: int`.

Поведения (4): `void touch()`, `void increaseHits()`, `bool isExpired(int, std::time_t) const`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getTrackId() const`, `double getSizeMb() const`, `std::time_t getLastAccessedAt() const`, `int getHits() const`.

Ассоциации (0): нет.

**CacheManager**

Назначение: Управление кэшем треков, вытеснение старых записей и статистика попаданий.

Наследование: нет.

Поля (5): `maxSizeMb: double`, `currentSizeMb: double`, `entries: std::vector<CacheEntry>`, `hitCount: int`, `missCount: int`.

Поведения (6): `void cacheTrack(const Track &, double)`, `bool evictOldest()`, `void clear()`, `bool isCached(int)`, `double hitRate() const`, `int optimize()`.

Доступ (геттеры/сеттеры, не учитываются): `double getMaxSizeMb() const`, `double getCurrentSizeMb() const`, `const std::vector<CacheEntry> & getEntries() const`, `int getHitCount() const`, `int getMissCount() const`.

Ассоциации (2): CacheEntry, Track.

### 4. Пользователи, подписка и платежи

**UserSettings**

Назначение: Настройки интерфейса и воспроизведения.

Наследование: нет.

Поля (6): `theme: std::string`, `language: std::string`, `crossfadeSeconds: int`, `normalizeVolume: bool`, `autoplay: bool`, `downloadOverWifi: bool`.

Поведения (8): `bool toggleAutoplay()`, `void changeCrossfade(int)`, `void resetToDefaults()`, `bool isDarkTheme() const`, `std::string summary() const`, `bool isNormalizeVolume() const`, `bool isAutoplay() const`, `bool isDownloadOverWifi() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getTheme() const`, `const std::string & getLanguage() const`, `int getCrossfadeSeconds() const`.

Ассоциации (0): нет.

**SubscriptionPlan**

Назначение: Тарифный план с ценой, лимитом устройств и возможностями.

Наследование: нет.

Поля (6): `name: std::string`, `pricePerMonth: double`, `maxDevices: int`, `adsFree: bool`, `offlineMode: bool`, `hiFiQuality: bool`.

Поведения (7): `bool allowsOffline() const`, `bool isFree() const`, `std::string describe() const`, `int compareWith(const SubscriptionPlan &) const`, `bool isAdsFree() const`, `bool isOfflineMode() const`, `bool isHiFiQuality() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getName() const`, `double getPricePerMonth() const`, `int getMaxDevices() const`.

Ассоциации (0): нет.

**Subscription**

Назначение: Подписка пользователя: план, срок, продление и устройства.

Наследование: нет.

Поля (5): `plan: SubscriptionPlan`, `startedAt: std::time_t`, `expiresAt: std::time_t`, `autoRenew: bool`, `active: bool`.

Поведения (8): `void renew(int)`, `void cancel()`, `bool isActive() const`, `bool canAddDevice(int) const`, `double monthlyCost() const`, `int daysLeft() const`, `void requireActive() const`, `bool isAutoRenew() const`.

Доступ (геттеры/сеттеры, не учитываются): `const SubscriptionPlan & getPlan() const`, `std::time_t getStartedAt() const`, `std::time_t getExpiresAt() const`.

Ассоциации (1): SubscriptionPlan.

**PaymentMethod**

Назначение: Банковская карта с проверкой срока действия и маскированием номера.

Наследование: нет.

Поля (5): `cardLast4: std::string`, `brand: std::string`, `expiryMonth: int`, `expiryYear: int`, `holder: std::string`.

Поведения (3): `void validate() const`, `bool isExpired(int, int) const`, `std::string maskedNumber() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getCardLast4() const`, `const std::string & getBrand() const`, `int getExpiryMonth() const`, `int getExpiryYear() const`, `const std::string & getHolder() const`.

Ассоциации (0): нет.

**Payment**

Назначение: Платёж за подписку с методом оплаты и статусом.

Наследование: нет.

Поля (7): `id: int`, `amount: double`, `currency: std::string`, `status: std::string`, `paidAt: std::time_t`, `method: PaymentMethod`, `failureReason: std::string`.

Поведения (5): `bool process()`, `double refund()`, `void markFailed(const std::string &)`, `std::string formattedAmount() const`, `bool isSuccessful() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getId() const`, `double getAmount() const`, `const std::string & getCurrency() const`, `const std::string & getStatus() const`, `std::time_t getPaidAt() const`, `const PaymentMethod & getMethod() const`, `const std::string & getFailureReason() const`.

Ассоциации (1): PaymentMethod.

**PromoCode**

Назначение: Промокод со скидкой, сроком действия и лимитом использований.

Наследование: нет.

Поля (5): `code: std::string`, `percent: int`, `validUntil: std::time_t`, `usageLimit: int`, `usedCount: int`.

Поведения (5): `bool isValid(std::time_t) const`, `double apply(double) const`, `bool redeem()`, `bool isExpired(std::time_t) const`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getCode() const`, `int getPercent() const`, `std::time_t getValidUntil() const`, `int getUsageLimit() const`, `int getUsedCount() const`.

Ассоциации (0): нет.

**InvoiceItem**

Назначение: Позиция счёта: описание, количество и цена.

Наследование: нет.

Поля (3): `description: std::string`, `quantity: int`, `unitPrice: double`.

Поведения (3): `double subtotal() const`, `double applyDiscount(double) const`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getDescription() const`, `int getQuantity() const`, `double getUnitPrice() const`.

Ассоциации (0): нет.

**Invoice**

Назначение: Счёт на оплату подписки с позициями, скидкой и сроком.

Наследование: нет.

Поля (6): `number: std::string`, `issuedAt: std::time_t`, `dueAt: std::time_t`, `items: std::vector<InvoiceItem>`, `paid: bool`, `discountPercent: double`.

Поведения (6): `void addItem(const InvoiceItem &)`, `double total() const`, `void markPaid()`, `bool isOverdue(std::time_t) const`, `void applyPromo(PromoCode &)`, `bool isPaid() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getNumber() const`, `std::time_t getIssuedAt() const`, `std::time_t getDueAt() const`, `const std::vector<InvoiceItem> & getItems() const`, `double getDiscountPercent() const`.

Ассоциации (2): InvoiceItem, PromoCode.

**UserProfile**

Назначение: Публичный профиль пользователя: имя, аватар, био и подписчики.

Наследование: нет.

Поля (5): `displayName: std::string`, `avatarUrl: std::string`, `bio: std::string`, `favoriteGenre: Genre`, `followersCount: int`.

Поведения (5): `void changeAvatar(const std::string &)`, `void updateBio(const std::string &)`, `void follow()`, `void unfollow()`, `std::string initials() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getDisplayName() const`, `const std::string & getAvatarUrl() const`, `const std::string & getBio() const`, `const Genre & getFavoriteGenre() const`, `int getFollowersCount() const`.

Ассоциации (1): Genre.

**User**

Назначение: Учётная запись с паролем, подпиской, профилем и настройками.

Наследование: нет.

Поля (9): `id: int`, `username: std::string`, `email: std::string`, `passwordHash: std::string`, `subscription: Subscription`, `profile: UserProfile`, `settings: UserSettings`, `createdAt: std::time_t`, `active: bool`.

Поведения (9): `bool authenticate(const std::string &) const`, `void changePassword(const std::string &, const std::string &)`, `bool upgrade(SubscriptionPlan)`, `bool isPremium() const`, `void deactivate()`, `void updateEmail(const std::string &)`, `std::string displayName() const`, `void requirePremium() const`, `bool isActive() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getId() const`, `const std::string & getUsername() const`, `const std::string & getEmail() const`, `const Subscription & getSubscription() const`, `const UserProfile & getProfile() const`, `const UserSettings & getSettings() const`, `std::time_t getCreatedAt() const`, `void setProfile(const UserProfile &)`, `void setSettings(const UserSettings &)`.

Ассоциации (4): Subscription, SubscriptionPlan, UserProfile, UserSettings.

### 5. Рекомендации, статистика и события

**Recommendation**

Назначение: Рекомендация трека с оценкой, причиной и источником.

Наследование: нет.

Поля (4): `track: Track`, `score: double`, `reason: std::string`, `source: std::string`.

Поведения (4): `bool isRelevant() const`, `std::string explain() const`, `void boost(double)`, `std::string describe() const`.

Доступ (геттеры/сеттеры, не учитываются): `const Track & getTrack() const`, `double getScore() const`, `const std::string & getReason() const`, `const std::string & getSource() const`.

Ассоциации (1): Track.

**RecommendationEngine**

Назначение: Движок рекомендаций по жанрам, похожести и весам.

Наследование: нет.

Поля (5): `library: const MusicLibrary *`, `history: const PlaybackHistory *`, `genreWeights: std::map<std::string, double>`, `diversityFactor: double`, `explicitAllowed: bool`.

Поведения (7): `std::vector<Recommendation> recommendByGenre(const Genre &, int)`, `std::vector<Recommendation> recommendSimilar(const Track &, int)`, `void updateWeights(const std::string &, double)`, `void filterExplicit(std::vector<Recommendation> &) const`, `std::vector<Recommendation> diversify(const std::vector<Recommendation> &, int) const`, `std::vector<Recommendation> topN(const std::vector<Recommendation> &, int) const`, `bool isExplicitAllowed() const`.

Доступ (геттеры/сеттеры, не учитываются): `const MusicLibrary * getLibrary() const`, `const PlaybackHistory * getHistory() const`, `const std::map<std::string, double> & getGenreWeights() const`, `double getDiversityFactor() const`.

Ассоциации (5): Genre, MusicLibrary, PlaybackHistory, Recommendation, Track.

**ListeningStatistics**

Назначение: Статистика прослушиваний по времени, жанрам и исполнителям.

Наследование: нет.

Поля (6): `totalMinutes: long long`, `playedTracks: int`, `topGenre: std::string`, `topArtist: std::string`, `streakDays: int`, `genreCounts: std::map<std::string, int>`.

Поведения (5): `long long record(const Track &)`, `double averageDaily(int) const`, `void reset()`, `std::string favoriteGenre() const`, `std::string summary() const`.

Доступ (геттеры/сеттеры, не учитываются): `long long getTotalMinutes() const`, `int getPlayedTracks() const`, `const std::string & getTopGenre() const`, `const std::string & getTopArtist() const`, `int getStreakDays() const`, `const std::map<std::string, int> & getGenreCounts() const`.

Ассоциации (1): Track.

**ChartEntry**

Назначение: Позиция трека в чарте, предыдущая позиция и число недель.

Наследование: нет.

Поля (4): `position: int`, `previousPosition: int`, `weeksOnChart: int`, `track: Track`.

Поведения (5): `void moveUp()`, `void moveDown()`, `bool isNew() const`, `std::string trend() const`, `void reposition(int)`.

Доступ (геттеры/сеттеры, не учитываются): `int getPosition() const`, `int getPreviousPosition() const`, `int getWeeksOnChart() const`, `const Track & getTrack() const`.

Ассоциации (1): Track.

**Chart**

Назначение: Музыкальный чарт за период с позициями и пересчётом мест.

Наследование: нет.

Поля (4): `name: std::string`, `period: std::string`, `entries: std::vector<ChartEntry>`, `updatedAt: std::time_t`.

Поведения (5): `void addEntry(const Track &)`, `void removeTrack(int)`, `void recalculatePositions()`, `int findTrackPosition(int) const`, `void clear()`.

Доступ (геттеры/сеттеры, не учитываются): `std::vector<ChartEntry> getTop(int) const`, `const std::string & getName() const`, `const std::string & getPeriod() const`, `const std::vector<ChartEntry> & getEntries() const`, `std::time_t getUpdatedAt() const`.

Ассоциации (2): ChartEntry, Track.

**Ticket**

Назначение: Билет на концерт с местом, ценой, QR-кодом и отметкой использования.

Наследование: нет.

Поля (5): `seat: std::string`, `price: double`, `purchased: bool`, `qrCode: std::string`, `used: bool`.

Поведения (7): `void validate() const`, `void markPurchased()`, `double refund()`, `void markUsed()`, `std::string describe() const`, `bool isPurchased() const`, `bool isUsed() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getSeat() const`, `double getPrice() const`, `const std::string & getQrCode() const`.

Ассоциации (0): нет.

**Concert**

Назначение: Концерт исполнителя с площадкой, датой, ценой и продажей билетов.

Наследование: нет.

Поля (6): `venue: std::string`, `city: std::string`, `date: std::time_t`, `ticketPrice: double`, `artist: Artist`, `soldTickets: int`.

Поведения (5): `bool isUpcoming(std::time_t) const`, `int daysUntil(std::time_t) const`, `std::string buyTicket(Ticket &)`, `std::string describe() const`, `bool isSoldOut(int) const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getVenue() const`, `const std::string & getCity() const`, `std::time_t getDate() const`, `double getTicketPrice() const`, `const Artist & getArtist() const`, `int getSoldTickets() const`.

Ассоциации (2): Artist, Ticket.

### 6. Контент и социальные функции

**RadioStation**

Назначение: Интернет-радиостанция с потоком, жанром и битрейтом.

Наследование: нет.

Поля (5): `name: std::string`, `streamUrl: std::string`, `genre: Genre`, `bitrateKbps: int`, `isLive: bool`.

Поведения (5): `std::string tuneIn() const`, `std::string tuneOut() const`, `bool isAvailable() const`, `std::string describe() const`, `bool isLiveStream() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getName() const`, `const std::string & getStreamUrl() const`, `const Genre & getGenre() const`, `int getBitrateKbps() const`.

Ассоциации (1): Genre.

**PodcastEpisode**

Назначение: Эпизод подкаста с аудиофайлом и отметкой прослушивания.

Наследование: нет.

Поля (6): `number: int`, `title: std::string`, `durationSeconds: int`, `publishedAt: std::time_t`, `audioFile: AudioFile`, `listened: bool`.

Поведения (5): `int play() const`, `void markListened()`, `double progressAfter(int) const`, `std::string formattedDuration() const`, `bool isListened() const`.

Доступ (геттеры/сеттеры, не учитываются): `int getNumber() const`, `const std::string & getTitle() const`, `int getDurationSeconds() const`, `std::time_t getPublishedAt() const`, `const AudioFile & getAudioFile() const`.

Ассоциации (1): AudioFile.

**Podcast**

Назначение: Подкаст с ведущим, категорией и списком эпизодов.

Наследование: нет.

Поля (5): `title: std::string`, `host: std::string`, `category: std::string`, `explicitContent: bool`, `episodes: std::vector<PodcastEpisode>`.

Поведения (5): `void addEpisode(const PodcastEpisode &)`, `const PodcastEpisode * latestEpisode() const`, `int totalDuration() const`, `int countListened() const`, `bool isExplicit() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getTitle() const`, `const std::string & getHost() const`, `const std::string & getCategory() const`, `const std::vector<PodcastEpisode> & getEpisodes() const`.

Ассоциации (1): PodcastEpisode.

**Chapter**

Назначение: Глава аудиокниги с временным диапазоном.

Наследование: нет.

Поля (4): `number: int`, `title: std::string`, `startSeconds: int`, `endSeconds: int`.

Поведения (4): `bool contains(int) const`, `int duration() const`, `std::string describe() const`, `bool overlap(const Chapter &) const`.

Доступ (геттеры/сеттеры, не учитываются): `int getNumber() const`, `const std::string & getTitle() const`, `int getStartSeconds() const`, `int getEndSeconds() const`.

Ассоциации (0): нет.

**Audiobook**

Назначение: Аудиокнига с главами, позицией воспроизведения и прогрессом.

Наследование: нет.

Поля (6): `title: std::string`, `author: std::string`, `narrator: std::string`, `chapters: std::vector<Chapter>`, `currentChapter: int`, `positionSeconds: int`.

Поведения (5): `void addChapter(const Chapter &)`, `void resumeAt(int)`, `int remainingSeconds() const`, `const Chapter * chapterAt(int) const`, `double progressPercent() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getTitle() const`, `const std::string & getAuthor() const`, `const std::string & getNarrator() const`, `const std::vector<Chapter> & getChapters() const`, `int getCurrentChapter() const`, `int getPositionSeconds() const`.

Ассоциации (1): Chapter.

**SocialPost**

Назначение: Публикация пользователя о треке с лайками и комментариями.

Наследование: нет.

Поля (6): `authorId: int`, `text: std::string`, `likes: int`, `createdAt: std::time_t`, `track: const Track *`, `comments: std::vector<std::string>`.

Поведения (6): `void like()`, `void unlike()`, `void addComment(const std::string &)`, `bool isPopular() const`, `std::string summary() const`, `bool authoredBy(const User &) const`.

Доступ (геттеры/сеттеры, не учитываются): `int getAuthorId() const`, `const std::string & getText() const`, `int getLikes() const`, `std::time_t getCreatedAt() const`, `const Track * getTrack() const`, `const std::vector<std::string> & getComments() const`.

Ассоциации (2): Track, User.

**Notification**

Назначение: Пользовательское уведомление с типом, важностью и временем.

Наследование: нет.

Поля (5): `title: std::string`, `message: std::string`, `type: std::string`, `read: bool`, `createdAt: std::time_t`.

Поведения (5): `void markRead()`, `bool isImportant() const`, `std::string summary() const`, `std::string ageDescription(std::time_t) const`, `bool isRead() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getTitle() const`, `const std::string & getMessage() const`, `const std::string & getType() const`, `std::time_t getCreatedAt() const`.

Ассоциации (0): нет.

### 7. Сервисы синхронизации и загрузки

**DeviceSync**

Назначение: Синхронизация библиотеки с внешним устройством.

Наследование: нет.

Поля (4): `deviceName: std::string`, `lastSyncAt: std::time_t`, `syncedTrackIds: std::vector<int>`, `enabled: bool`.

Поведения (5): `int sync(const std::vector<Track> &)`, `void disconnect()`, `bool needsSync(std::time_t, int) const`, `int syncedCount() const`, `bool isEnabled() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getDeviceName() const`, `std::time_t getLastSyncAt() const`, `const std::vector<int> & getSyncedTrackIds() const`.

Ассоциации (1): Track.

**Scrobbler**

Назначение: Отправка прослушиваний во внешний сервис (скробблинг).

Наследование: нет.

Поля (4): `serviceName: std::string`, `enabled: bool`, `pending: std::vector<Track>`, `submittedCount: int`.

Поведения (5): `bool scrobble(const Track &)`, `int flush()`, `bool toggle()`, `int pendingCount() const`, `bool isEnabled() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::string & getServiceName() const`, `const std::vector<Track> & getPending() const`, `int getSubmittedCount() const`.

Ассоциации (1): Track.

**DownloadTask**

Назначение: Задача загрузки трека с прогрессом, статусом и ошибкой.

Наследование: нет.

Поля (6): `track: Track`, `status: std::string`, `progressPercent: int`, `startedAt: std::time_t`, `errorMessage: std::string`, `sizeMb: double`.

Поведения (6): `void start()`, `void updateProgress(int)`, `void complete()`, `void fail(const std::string &)`, `void cancel()`, `bool isDone() const`.

Доступ (геттеры/сеттеры, не учитываются): `const Track & getTrack() const`, `const std::string & getStatus() const`, `int getProgressPercent() const`, `std::time_t getStartedAt() const`, `const std::string & getErrorMessage() const`, `double getSizeMb() const`.

Ассоциации (1): Track.

**DownloadManager**

Назначение: Очередь загрузок с лимитом хранилища и обработкой задач.

Наследование: нет.

Поля (4): `tasks: std::vector<DownloadTask>`, `storageLimitMb: double`, `usedMb: double`, `wifiOnly: bool`.

Поведения (7): `int queueDownload(const Track &, double)`, `void cancel(int)`, `int process()`, `double totalSize() const`, `int clearCompleted()`, `double freeSpace() const`, `bool isWifiOnly() const`.

Доступ (геттеры/сеттеры, не учитываются): `const std::vector<DownloadTask> & getTasks() const`, `double getStorageLimitMb() const`, `double getUsedMb() const`.

Ассоциации (2): DownloadTask, Track.

## 5. Собственные исключения (19)

| Исключение | Базовый класс | Код | Назначение |
|---|---|---|---|
| MusicPlayerException | std::runtime_error | MP-000 | Базовое исключение домена. |
| TrackNotFoundException | MusicPlayerException | MP-101 | Трек не найден. |
| InvalidTrackDataException | MusicPlayerException | MP-102 | Некорректные метаданные или аудиофайл. |
| DuplicateTrackException | MusicPlayerException | MP-103 | Дублирование трека. |
| PlaylistLimitException | MusicPlayerException | MP-104 | Превышен лимит треков в плейлисте. |
| EmptyQueueException | MusicPlayerException | MP-105 | Очередь воспроизведения пуста. |
| AuthenticationException | MusicPlayerException | MP-201 | Ошибка аутентификации. |
| InvalidPasswordException | MusicPlayerException | MP-202 | Некорректный пароль. |
| SubscriptionException | MusicPlayerException | MP-203 | Ошибка подписки. |
| SubscriptionExpiredException | MusicPlayerException | MP-204 | Подписка неактивна. |
| PaymentException | MusicPlayerException | MP-301 | Ошибка платежа или карты. |
| InvalidPromoCodeException | MusicPlayerException | MP-302 | Промокод недействителен. |
| DownloadException | MusicPlayerException | MP-401 | Ошибка загрузки. |
| StorageLimitException | MusicPlayerException | MP-402 | Недостаточно места. |
| AudioDeviceException | MusicPlayerException | MP-501 | Ошибка аудиоустройства. |
| DeviceNotConnectedException | MusicPlayerException | MP-502 | Устройство не подключено. |
| InvalidVolumeException | MusicPlayerException | MP-503 | Недопустимая громкость. |
| RadioUnavailableException | MusicPlayerException | MP-601 | Радиостанция недоступна. |
| TicketException | MusicPlayerException | MP-701 | Ошибка билета. |

Иерархия: `MusicPlayerException` наследуется от `std::runtime_error`; остальные исключения наследуются от `MusicPlayerException` и переопределяют виртуальный метод `code()`. Это позволяет ловить как конкретные ошибки, так и весь домен целиком.

## 6. Структура проекта

```text
3labapp/
├── CMakeLists.txt
├── README.md
├── include/musicplayer/
│   ├── audio.hpp        # аудиотека и метаданные
│   ├── playback.hpp     # воспроизведение и звук
│   ├── library.hpp      # библиотека, поиск, кэш
│   ├── user.hpp         # пользователи, подписка, платежи
│   ├── content.hpp      # рекомендации, контент, сервисы
│   ├── exceptions.hpp   # 19 собственных исключений
│   ├── util.hpp         # строковые и временные утилиты
│   └── music_player.hpp # umbrella-заголовок
└── src/main.cpp         # демонстрационный сценарий
```

## 7. Проверка требований

| Требование | Минимум | Факт |
|---|---:|---:|
| Классы | 50 | 56 |
| Поля | 150 | 275 |
| Поведения | 100 | 322 |
| Ассоциации | 30 | 62 |
| Исключения | 12 | 19 |

Демонстрационный сценарий в `src/main.cpp` покрывает все подсистемы и специально вызывает два исключения (`InvalidVolumeException`, `DuplicateTrackException`), чтобы показать обработку ошибок.
