#include "Player.h"
#include "Utils/Globals.h"
#include <cstdlib>

/* ---------- Private methods ---------- */
void Player::correct_position(Rlib::Rectangle& rec,
                              const Rlib::Rectangle& bounds, const Axis axis) {
    switch (axis) {
        using enum Axis;
    case HORIZONTAL:
        if (rec.x < bounds.x)
            rec.x = bounds.x;
        else if (rec.x + rec.width > bounds.x + bounds.width)
            rec.x = bounds.x + bounds.width - rec.width;
        break;

    case VERTICAL:
        if (rec.y < bounds.y)
            rec.y = bounds.y;
        else if (rec.y + rec.height > bounds.y + bounds.height)
            rec.y = bounds.y + bounds.height - rec.height;
        break;

    default:
        break;
    }
};

/* ---------- Public methods ---------- */
Player::Player(const Rlib::Rectangle& hitbox, const TrashInfo::Type type,
               const float speed, const int max_health)
    : m_hitbox{hitbox}, speed{speed}, m_type{type}, m_max_health{max_health} {
    m_color = TrashInfo::colors_map[type];
    m_sprites = {Config::General::assets_path + "/images/spaceship.png",
                 {2, 1}};
    m_sprite_box = Rlib::Rectangle{
        {m_hitbox.x, m_hitbox.y},
        {static_cast<float>(Config::General::spaceship_size.x),
         static_cast<float>(Config::General::spaceship_size.y)}};
    this->reset();
};

void Player::update(const Rlib::Rectangle& bounds) {
    this->correct_position(m_hitbox, bounds, Axis::HORIZONTAL);
    this->correct_position(m_hitbox, bounds, Axis::VERTICAL);

    m_sprite_box = Rlib::Rectangle{
        {m_hitbox.x - std::abs(m_sprite_box.width - m_hitbox.width) / 2,
         m_hitbox.y - std::abs(m_sprite_box.height - m_hitbox.height) / 2},
        {static_cast<float>(Config::General::spaceship_size.x),
         static_cast<float>(Config::General::spaceship_size.y)}};

    this->m_sprites.update();

    this->correct_position(m_sprite_box, bounds, Axis::HORIZONTAL);
    this->correct_position(m_sprite_box, bounds, Axis::VERTICAL);
};

void Player::update_position(const Rlib::Rectangle& bounds) {
    this->place_at_bottom(bounds);

    this->update(bounds);
};

void Player::move(const Direction direction, const Rlib::Rectangle& bounds,
                  const float dt) {
    switch (direction) {
        using enum Direction;
    case UP:
        m_hitbox.y -= speed * dt;
        this->correct_position(m_hitbox, bounds, Axis::VERTICAL);
        break;

    case DOWN:
        m_hitbox.y += speed * dt;
        this->correct_position(m_hitbox, bounds, Axis::VERTICAL);
        break;

    case LEFT:
        m_hitbox.x -= speed * dt;
        this->correct_position(m_hitbox, bounds, Axis::HORIZONTAL);
        break;

    case RIGHT:
        m_hitbox.x += speed * dt;
        this->correct_position(m_hitbox, bounds, Axis::HORIZONTAL);
        break;

    default:
        break;
    }
};

void Player::set_type(const TrashInfo::Type type) {
    m_type = type;
    m_color = TrashInfo::colors_map[type];
};

void Player::handle_input(const Rlib::Rectangle& bounds, const float dt) {
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        this->move(Direction::UP, bounds, dt);

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        this->move(Direction::LEFT, bounds, dt);

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        this->move(Direction::DOWN, bounds, dt);

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        this->move(Direction::RIGHT, bounds, dt);

    int key = GetKeyPressed();

    switch (key) {
    case KEY_ONE:
    case KEY_KP_1:
        this->set_type(TrashInfo::Type::PAPER);
        break;

    case KEY_TWO:
    case KEY_KP_2:
        this->set_type(TrashInfo::Type::GLASS);
        break;

    case KEY_THREE:
    case KEY_KP_3:
        this->set_type(TrashInfo::Type::PLASTIC);
        break;

    case KEY_FOUR:
    case KEY_KP_4:
        this->set_type(TrashInfo::Type::METAL);
        break;

    case KEY_FIVE:
    case KEY_KP_5:
        this->set_type(TrashInfo::Type::ORGANIC);
        break;

    default:
        break;
    }
};

void Player::modify_points(const Math::Operations op, const int amount) {
    switch (op) {
        using enum Math::Operations;
    case ADD:
        m_score += amount;
        break;

    case SUB:
        m_score -= amount;
        break;

    case MULT:
        m_score *= amount;
        break;

    case DIV:
        m_score /= amount;
        break;

    default:
        break;
    }
};

void Player::modify_health(const Math::Operations op, const int amount) {
    switch (op) {
        using enum Math::Operations;
    case ADD:
        m_health += amount;
        break;

    case SUB:
        m_health -= amount;
        break;

    case MULT:
        m_health *= amount;
        break;

    case DIV:
        m_health /= amount;
        break;

    default:
        break;
    }

    if (m_health < 0)
        m_health = 0;
};

bool Player::is_alive() { return m_health > 0; };

void Player::reset() {
    m_score = 0;
    m_health = m_max_health;
};

void Player::place_in_middle(const Rlib::Rectangle bounds) {
    m_hitbox.x =
        get_middle_rec(bounds).x - Config::General::spaceship_hitbox.x / 2.f;

    m_hitbox.y =
        get_middle_rec(bounds).y - (Config::General::spaceship_hitbox.y) / 2.f;
};

bool Player::is_same_type(const Trash& enemy) const {
    return enemy.get_type() == m_type;
};

bool Player::is_correct_enemy(const Trash& enemy) const {
    return is_enemy_colliding(enemy.get_hitbox()) && enemy.get_type() == m_type;
};

bool Player::is_enemy_colliding(const Rlib::Rectangle& enemy_hitbox) const {
    return CheckCollisionRecs(m_hitbox, enemy_hitbox);
};

void Player::place_at_bottom(const Rlib::Rectangle bounds) {
    m_hitbox.y = bounds.y + bounds.height - m_hitbox.height - 30;
};

void Player::draw() {
    // m_sprite_box.Draw(PURPLE);

    m_sprites.draw(m_sprite_box);

    // m_hitbox.Draw(m_color);

    // Player sprite animation
    if (m_sprites.is_ready()) {
        m_sprites.rec_advance();
        m_sprites.reset_frame_counter();
    }
};
