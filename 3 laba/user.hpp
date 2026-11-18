#pragma once

#include "audio.hpp"
#include "exceptions.hpp"
#include "util.hpp"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <string>
#include <utility>
#include <vector>

namespace mp {

class UserSettings {
    std::string theme = "dark";
    std::string language = "ru";
    int crossfadeSeconds = 0;
    bool normalizeVolume = true;
    bool autoplay = true;
    bool downloadOverWifi = true;

public:
    UserSettings() = default;

    bool toggleAutoplay() {
        autoplay = !autoplay;
        return autoplay;
    }

    void changeCrossfade(int seconds) {
        if (seconds < 0 || seconds > 12) {
            throw MusicPlayerException("Crossfade must be in range 0..12 seconds");
        }
        crossfadeSeconds = seconds;
    }

    void resetToDefaults() {
        theme = "dark";
        language = "ru";
        crossfadeSeconds = 0;
        normalizeVolume = true;
        autoplay = true;
        downloadOverWifi = true;
    }

    bool isDarkTheme() const {
        return theme == "dark";
    }

    std::string summary() const {
        return "theme=" + theme + ", lang=" + language + ", crossfade=" +
               std::to_string(crossfadeSeconds) + "s, autoplay=" + (autoplay ? "on" : "off");
    }

    const std::string& getTheme() const { return theme; }
    const std::string& getLanguage() const { return language; }
    int getCrossfadeSeconds() const { return crossfadeSeconds; }
    bool isNormalizeVolume() const { return normalizeVolume; }
    bool isAutoplay() const { return autoplay; }
    bool isDownloadOverWifi() const { return downloadOverWifi; }
};

class SubscriptionPlan {
    std::string name;
    double pricePerMonth = 0.0;
    int maxDevices = 1;
    bool adsFree = false;
    bool offlineMode = false;
    bool hiFiQuality = false;

public:
    SubscriptionPlan() = default;

    SubscriptionPlan(std::string name, double pricePerMonth, int maxDevices, bool adsFree,
                     bool offlineMode, bool hiFiQuality)
        : name(std::move(name)),
          pricePerMonth(pricePerMonth),
          maxDevices(maxDevices),
          adsFree(adsFree),
          offlineMode(offlineMode),
          hiFiQuality(hiFiQuality) {}

    bool allowsOffline() const {
        return offlineMode;
    }

    bool isFree() const {
        return pricePerMonth <= 0.0;
    }

    std::string describe() const {
        return name + ": " + std::to_string(pricePerMonth) + " per month, " +
               std::to_string(maxDevices) + " devices" + (adsFree ? ", no ads" : ", with ads") +
               (hiFiQuality ? ", Hi-Fi" : "");
    }

    int compareWith(const SubscriptionPlan& other) const {
        if (pricePerMonth < other.pricePerMonth) {
            return -1;
        }
        if (pricePerMonth > other.pricePerMonth) {
            return 1;
        }
        return 0;
    }

    const std::string& getName() const { return name; }
    double getPricePerMonth() const { return pricePerMonth; }
    int getMaxDevices() const { return maxDevices; }
    bool isAdsFree() const { return adsFree; }
    bool isOfflineMode() const { return offlineMode; }
    bool isHiFiQuality() const { return hiFiQuality; }
};

class Subscription {
    SubscriptionPlan plan;
    std::time_t startedAt = 0;
    std::time_t expiresAt = 0;
    bool autoRenew = true;
    bool active = false;

public:
    Subscription() = default;

    Subscription(SubscriptionPlan plan, int durationDays = 30)
        : plan(std::move(plan)),
          startedAt(util::now()),
          expiresAt(util::now() + static_cast<long>(durationDays) * 86400L),
          autoRenew(true),
          active(true) {}

    void renew(int months) {
        if (months <= 0) {
            throw SubscriptionException("Renewal period must be positive");
        }
        const std::time_t base = std::max(expiresAt, util::now());
        expiresAt = base + static_cast<long>(months) * 30L * 86400L;
        active = true;
        autoRenew = true;
    }

    void cancel() {
        autoRenew = false;
    }

    bool isActive() const {
        return active && expiresAt > util::now();
    }

    bool canAddDevice(int currentDevices) const {
        return isActive() && currentDevices < plan.getMaxDevices();
    }

    double monthlyCost() const {
        return plan.getPricePerMonth();
    }

    int daysLeft() const {
        const long long difference = static_cast<long long>(expiresAt) -
                                     static_cast<long long>(util::now());
        return difference <= 0 ? 0 : static_cast<int>(difference / 86400);
    }

    void requireActive() const {
        if (!isActive()) {
            throw SubscriptionExpiredException("Subscription is not active");
        }
    }

    const SubscriptionPlan& getPlan() const { return plan; }
    std::time_t getStartedAt() const { return startedAt; }
    std::time_t getExpiresAt() const { return expiresAt; }
    bool isAutoRenew() const { return autoRenew; }
};

class PaymentMethod {
    std::string cardLast4;
    std::string brand;
    int expiryMonth = 1;
    int expiryYear = 2000;
    std::string holder;

public:
    PaymentMethod() = default;

    PaymentMethod(std::string cardLast4, std::string brand, int expiryMonth, int expiryYear,
                  std::string holder)
        : cardLast4(std::move(cardLast4)),
          brand(std::move(brand)),
          expiryMonth(expiryMonth),
          expiryYear(expiryYear),
          holder(std::move(holder)) {}

    void validate() const {
        if (cardLast4.size() != 4 ||
            !std::all_of(cardLast4.begin(), cardLast4.end(),
                         [](unsigned char c) { return std::isdigit(c) != 0; })) {
            throw PaymentException("Card number must contain exactly four digits");
        }
        if (brand.empty() || holder.empty()) {
            throw PaymentException("Payment method is incomplete");
        }
        if (expiryMonth < 1 || expiryMonth > 12) {
            throw PaymentException("Expiry month must be in range 1..12");
        }
        const std::time_t current = util::now();
        const std::tm* local = std::localtime(&current);
        if (local == nullptr || isExpired(local->tm_year + 1900, local->tm_mon + 1)) {
            throw PaymentException("Payment card is expired");
        }
    }

    bool isExpired(int currentYear, int currentMonth) const {
        return expiryYear < currentYear ||
               (expiryYear == currentYear && expiryMonth < currentMonth);
    }

    std::string maskedNumber() const {
        return "**** **** **** " + cardLast4;
    }

    const std::string& getCardLast4() const { return cardLast4; }
    const std::string& getBrand() const { return brand; }
    int getExpiryMonth() const { return expiryMonth; }
    int getExpiryYear() const { return expiryYear; }
    const std::string& getHolder() const { return holder; }
};

class Payment {
    int id = 0;
    double amount = 0.0;
    std::string currency = "USD";
    std::string status = "pending";
    std::time_t paidAt = 0;
    PaymentMethod method;
    std::string failureReason;

public:
    Payment() = default;

    Payment(int id, double amount, std::string currency, PaymentMethod method)
        : id(id),
          amount(amount),
          currency(std::move(currency)),
          method(std::move(method)) {}

    bool process() {
        if (amount <= 0) {
            throw PaymentException("Payment amount must be positive");
        }
        method.validate();
        status = "paid";
        paidAt = util::now();
        failureReason.clear();
        return true;
    }

    double refund() {
        if (status != "paid") {
            throw PaymentException("Only a paid transaction can be refunded");
        }
        status = "refunded";
        return amount;
    }

    void markFailed(const std::string& reason) {
        status = "failed";
        failureReason = reason;
    }

    std::string formattedAmount() const {
        return std::to_string(amount) + " " + currency;
    }

    bool isSuccessful() const {
        return status == "paid";
    }

    int getId() const { return id; }
    double getAmount() const { return amount; }
    const std::string& getCurrency() const { return currency; }
    const std::string& getStatus() const { return status; }
    std::time_t getPaidAt() const { return paidAt; }
    const PaymentMethod& getMethod() const { return method; }
    const std::string& getFailureReason() const { return failureReason; }
};

class PromoCode {
    std::string code;
    int percent = 0;
    std::time_t validUntil = 0;
    int usageLimit = 0;
    int usedCount = 0;

public:
    PromoCode() = default;

    PromoCode(std::string code, int percent, std::time_t validUntil, int usageLimit)
        : code(std::move(code)),
          percent(percent),
          validUntil(validUntil),
          usageLimit(usageLimit) {}

    bool isValid(std::time_t currentTime) const {
        return !code.empty() && percent >= 1 && percent <= 100 && usageLimit > 0 &&
               usedCount < usageLimit && validUntil > currentTime;
    }

    double apply(double amount) const {
        if (amount < 0) {
            throw InvalidPromoCodeException("Amount cannot be negative");
        }
        if (!isValid(util::now())) {
            throw InvalidPromoCodeException("Promo code " + code + " is invalid or expired");
        }
        return amount * (100 - percent) / 100.0;
    }

    bool redeem() {
        if (!isValid(util::now())) {
            throw InvalidPromoCodeException("Promo code " + code + " cannot be redeemed");
        }
        ++usedCount;
        return true;
    }

    bool isExpired(std::time_t currentTime) const {
        return validUntil <= currentTime;
    }

    std::string describe() const {
        return code + ": " + std::to_string(percent) + "% off, used " +
               std::to_string(usedCount) + "/" + std::to_string(usageLimit);
    }

    const std::string& getCode() const { return code; }
    int getPercent() const { return percent; }
    std::time_t getValidUntil() const { return validUntil; }
    int getUsageLimit() const { return usageLimit; }
    int getUsedCount() const { return usedCount; }
};

class InvoiceItem {
    std::string description;
    int quantity = 1;
    double unitPrice = 0.0;

public:
    InvoiceItem() = default;

    InvoiceItem(std::string description, int quantity, double unitPrice)
        : description(std::move(description)),
          quantity(quantity),
          unitPrice(unitPrice) {}

    double subtotal() const {
        return quantity * unitPrice;
    }

    double applyDiscount(double percent) const {
        if (percent < 0 || percent > 100) {
            throw MusicPlayerException("Discount must be in range 0..100");
        }
        return subtotal() * (100.0 - percent) / 100.0;
    }

    std::string describe() const {
        return description + " x" + std::to_string(quantity) + " = " +
               std::to_string(subtotal());
    }

    const std::string& getDescription() const { return description; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
};

class Invoice {
    std::string number;
    std::time_t issuedAt = 0;
    std::time_t dueAt = 0;
    std::vector<InvoiceItem> items;
    bool paid = false;
    double discountPercent = 0.0;

public:
    Invoice() = default;

    Invoice(std::string number, int dueDays = 14)
        : number(std::move(number)),
          issuedAt(util::now()),
          dueAt(util::now() + static_cast<long>(dueDays) * 86400L) {}

    void addItem(const InvoiceItem& item) {
        if (item.getQuantity() <= 0 || item.getUnitPrice() < 0) {
            throw PaymentException("Invoice item has invalid quantity or price");
        }
        items.push_back(item);
    }

    double total() const {
        double sum = 0.0;
        for (const InvoiceItem& item : items) {
            sum += item.subtotal();
        }
        return sum * (100.0 - discountPercent) / 100.0;
    }

    void markPaid() {
        paid = true;
    }

    bool isOverdue(std::time_t currentTime) const {
        return !paid && dueAt < currentTime;
    }

    void applyPromo(PromoCode& promo) {
        promo.redeem();
        discountPercent = promo.getPercent();
    }

    const std::string& getNumber() const { return number; }
    std::time_t getIssuedAt() const { return issuedAt; }
    std::time_t getDueAt() const { return dueAt; }
    const std::vector<InvoiceItem>& getItems() const { return items; }
    bool isPaid() const { return paid; }
    double getDiscountPercent() const { return discountPercent; }
};

class UserProfile {
    std::string displayName;
    std::string avatarUrl;
    std::string bio;
    Genre favoriteGenre;
    int followersCount = 0;

public:
    UserProfile() = default;

    UserProfile(std::string displayName, Genre favoriteGenre)
        : displayName(std::move(displayName)), favoriteGenre(std::move(favoriteGenre)) {}

    void changeAvatar(const std::string& url) {
        if (url.empty()) {
            throw MusicPlayerException("Avatar URL cannot be empty");
        }
        avatarUrl = url;
    }

    void updateBio(const std::string& value) {
        if (value.size() > 300) {
            throw MusicPlayerException("Bio cannot be longer than 300 characters");
        }
        bio = value;
    }

    void follow() {
        ++followersCount;
    }

    void unfollow() {
        if (followersCount > 0) {
            --followersCount;
        }
    }

    std::string initials() const {
        std::string result;
        bool newWord = true;
        for (unsigned char c : displayName) {
            if (std::isalpha(c) && newWord) {
                result += static_cast<char>(std::toupper(c));
                newWord = false;
            } else if (!std::isalpha(c)) {
                newWord = true;
            }
            if (result.size() == 2) {
                break;
            }
        }
        return result;
    }

    const std::string& getDisplayName() const { return displayName; }
    const std::string& getAvatarUrl() const { return avatarUrl; }
    const std::string& getBio() const { return bio; }
    const Genre& getFavoriteGenre() const { return favoriteGenre; }
    int getFollowersCount() const { return followersCount; }
};

class User {
    int id = 0;
    std::string username;
    std::string email;
    std::string passwordHash;
    Subscription subscription;
    UserProfile profile;
    UserSettings settings;
    std::time_t createdAt = 0;
    bool active = true;

public:
    User() = default;

    User(int id, std::string username, std::string email, const std::string& password,
         UserProfile profile = UserProfile())
        : id(id),
          username(std::move(username)),
          email(std::move(email)),
          passwordHash(util::simpleHash(password)),
          profile(std::move(profile)),
          createdAt(util::now()) {}

    bool authenticate(const std::string& password) const {
        if (!active) {
            throw AuthenticationException("Account is deactivated");
        }
        if (util::simpleHash(password) != passwordHash) {
            throw AuthenticationException("Invalid password for user " + username);
        }
        return true;
    }

    void changePassword(const std::string& oldPassword, const std::string& newPassword) {
        authenticate(oldPassword);
        if (newPassword.size() < 8) {
            throw InvalidPasswordException("New password must contain at least 8 characters");
        }
        if (newPassword == oldPassword) {
            throw InvalidPasswordException("New password must differ from the old password");
        }
        passwordHash = util::simpleHash(newPassword);
    }

    bool upgrade(SubscriptionPlan plan) {
        if (!active) {
            throw AuthenticationException("Cannot upgrade a deactivated account");
        }
        if (plan.getPricePerMonth() < 0) {
            throw SubscriptionException("Subscription price cannot be negative");
        }
        subscription = Subscription(std::move(plan), 30);
        return subscription.isActive();
    }

    bool isPremium() const {
        return subscription.isActive() && !subscription.getPlan().isFree();
    }

    void deactivate() {
        active = false;
    }

    void updateEmail(const std::string& value) {
        if (value.find('@') == std::string::npos || value.find('.') == std::string::npos) {
            throw AuthenticationException("Email format is invalid");
        }
        email = value;
    }

    std::string displayName() const {
        return profile.getDisplayName().empty() ? username : profile.getDisplayName();
    }

    void requirePremium() const {
        if (!isPremium()) {
            throw SubscriptionExpiredException("Premium subscription is required");
        }
    }

    int getId() const { return id; }
    const std::string& getUsername() const { return username; }
    const std::string& getEmail() const { return email; }
    const Subscription& getSubscription() const { return subscription; }
    const UserProfile& getProfile() const { return profile; }
    const UserSettings& getSettings() const { return settings; }
    std::time_t getCreatedAt() const { return createdAt; }
    bool isActive() const { return active; }
    void setProfile(const UserProfile& value) { profile = value; }
    void setSettings(const UserSettings& value) { settings = value; }
};

} // namespace mp
