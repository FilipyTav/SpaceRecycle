#include "Player.h"

/* ---------- Private methods ---------- */
void Player::correct_position(const Rlib::Rectangle& bounds, const Axis axis) {
    switch (axis) {
        using enum Axis;
    case HORIZONTAL:
        if (m_hitbox.x < bounds.x)
            m_hitbox.x = bounds.x;
        else if (m_hitbox.x + m_hitbox.width > bounds.x + bounds.width)
            m_hitbox.x = bounds.x + bounds.width - m_hitbox.width;
        break;

    case VERTICAL:
        if (m_hitbox.y < bounds.y)
            m_hitbox.y = bounds.y;
        else if (m_hitbox.y + m_hitbox.height > bounds.y + bounds.height)
            m_hitbox.y = bounds.y + bounds.height - m_hitbox.height;
        break;

    default:
        break;
    }
};

/* ---------- Public methods ---------- */

Player::Player(const Rlib::Rectangle& hitbox, const TrashInfo::Type type,
               const float speed, const int health)
    : m_hitbox{hitbox}, speed{speed}, m_type{type}, m_health{health} {
    m_color = TrashInfo::colors_map[type];
};

void Player::update(const Rlib::Rectangle& bounds) {
    // Constant height
    m_hitbox.y = bounds.y + bounds.height - m_hitbox.height - 30;

    this->correct_position(bounds, Axis::HORIZONTAL);
    this->correct_position(bounds, Axis::VERTICAL);
};

void Player::move(const Direction direction, const Rlib::Rectangle& bounds,
                  const float dt) {
    switch (direction) {
        using enum Direction;
    case UP:
    case DOWN:
        break;

    case LEFT:
        m_hitbox.x -= speed * dt;
        this->correct_position(bounds, Axis::HORIZONTAL);

        break;

    case RIGHT:
        m_hitbox.x += speed * dt;
        this->correct_position(bounds, Axis::HORIZONTAL);

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
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        this->move(Direction::LEFT, bounds, dt);

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
