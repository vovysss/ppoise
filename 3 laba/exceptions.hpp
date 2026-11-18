#pragma once

#include <stdexcept>
#include <string>

namespace mp {

class MusicPlayerException : public std::runtime_error {
public:
    explicit MusicPlayerException(const std::string& message)
        : std::runtime_error(message) {}

    virtual const char* code() const noexcept { return "MP-000"; }
};

class TrackNotFoundException : public MusicPlayerException {
public:
    explicit TrackNotFoundException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-101"; }
};

class InvalidTrackDataException : public MusicPlayerException {
public:
    explicit InvalidTrackDataException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-102"; }
};

class DuplicateTrackException : public MusicPlayerException {
public:
    explicit DuplicateTrackException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-103"; }
};

class PlaylistLimitException : public MusicPlayerException {
public:
    explicit PlaylistLimitException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-104"; }
};

class EmptyQueueException : public MusicPlayerException {
public:
    explicit EmptyQueueException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-105"; }
};

class AuthenticationException : public MusicPlayerException {
public:
    explicit AuthenticationException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-201"; }
};

class InvalidPasswordException : public MusicPlayerException {
public:
    explicit InvalidPasswordException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-202"; }
};

class SubscriptionException : public MusicPlayerException {
public:
    explicit SubscriptionException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-203"; }
};

class SubscriptionExpiredException : public MusicPlayerException {
public:
    explicit SubscriptionExpiredException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-204"; }
};

class PaymentException : public MusicPlayerException {
public:
    explicit PaymentException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-301"; }
};

class InvalidPromoCodeException : public MusicPlayerException {
public:
    explicit InvalidPromoCodeException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-302"; }
};

class DownloadException : public MusicPlayerException {
public:
    explicit DownloadException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-401"; }
};

class StorageLimitException : public MusicPlayerException {
public:
    explicit StorageLimitException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-402"; }
};

class AudioDeviceException : public MusicPlayerException {
public:
    explicit AudioDeviceException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-501"; }
};

class DeviceNotConnectedException : public MusicPlayerException {
public:
    explicit DeviceNotConnectedException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-502"; }
};

class InvalidVolumeException : public MusicPlayerException {
public:
    explicit InvalidVolumeException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-503"; }
};

class RadioUnavailableException : public MusicPlayerException {
public:
    explicit RadioUnavailableException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-601"; }
};

class TicketException : public MusicPlayerException {
public:
    explicit TicketException(const std::string& message)
        : MusicPlayerException(message) {}
    const char* code() const noexcept override { return "MP-701"; }
};

} // namespace mp
