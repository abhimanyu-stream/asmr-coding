#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

class Particle
{
public:
    sf::CircleShape shape;
    sf::Vector3f position; // 3D position (x, y, z)
    sf::Vector2f velocity;
    bool isSatellite;

    Particle(float x, float y, float z, float radius, sf::Color color, sf::Vector2f initialVelocity, bool satellite)
        : position(x, y, z), velocity(initialVelocity), isSatellite(satellite)
    {
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius); // Center the shape
        shape.setPosition(position.x, position.y);
    }

    void update(float deltaTime, float centerX, float centerY)
    {
        if (isSatellite)
        {
            // Circular motion in 3D space
            float angle = std::atan2(position.y - centerY, position.x - centerX);
            angle += velocity.x * deltaTime;              // Angular velocity
            position.x = centerX + 200 * std::cos(angle); // 200 is orbit radius
            position.y = centerY + 200 * std::sin(angle);
            position.z = 200 + 50 * std::sin(angle * 2); // Add a 3D wobble effect
        }
        else
        {
            // Zig-zag motion
            position.x += velocity.x * deltaTime * 50;
            position.y += velocity.y * deltaTime * 50;
            position.z += velocity.x * deltaTime * 10; // Simulate depth movement

            // Wrap around the screen
            if (position.x < 0)
                position.x = 800;
            if (position.x > 800)
                position.x = 0;
            if (position.y < 0)
                position.y = 600;
            if (position.y > 600)
                position.y = 0;
            if (position.z < 0)
                position.z = 400; // Reset depth
        }

        // Perspective effect: Adjust size and brightness based on z-depth
        float scale = 1.0f / (0.5f + position.z / 400.0f); // Simulate perspective scaling
        shape.setScale(scale, scale);

        sf::Color color = shape.getFillColor();
        int brightness = static_cast<int>(200 * scale);
        color.r = brightness;
        color.g = brightness;
        color.b = brightness;
        shape.setFillColor(color);

        shape.setPosition(position.x, position.y);
    }
};

int main()
{
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Space Particle Simulation");
    window.setFramerateLimit(60);

    // Create particles
    std::vector<Particle> particles;
    for (int i = 0; i < 50; ++i)
    {
        float x = std::rand() % 800;
        float y = std::rand() % 600;
        float z = std::rand() % 400; // Random depth
        float radius = 5.0f + std::rand() % 5;
        sf::Color color = sf::Color::White;
        sf::Vector2f velocity(
            (std::rand() % 200 - 100) / 100.0f,
            (std::rand() % 200 - 100) / 100.0f);

        bool isSatellite = (i < 10); // First 10 particles are satellites
        particles.emplace_back(x, y, z, radius, color, velocity, isSatellite);
    }

    // Create static background stars
    sf::VertexArray stars(sf::Points, 100);
    for (size_t i = 0; i < stars.getVertexCount(); ++i)
    {
        stars[i].position = sf::Vector2f(std::rand() % 800, std::rand() % 600);
        stars[i].color = sf::Color(150, 150, 150);
    }

    // Center of the screen (for satellites)
    float centerX = 400.0f;
    float centerY = 300.0f;

    // Orbit path for satellites
    sf::CircleShape orbitPath(200.0f);
    orbitPath.setOrigin(200.0f, 200.0f);
    orbitPath.setPosition(centerX, centerY);
    orbitPath.setOutlineColor(sf::Color(100, 100, 100));
    orbitPath.setOutlineThickness(1.0f);
    orbitPath.setFillColor(sf::Color::Transparent);

    // Clock for delta time
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();

        // Update particles
        for (auto &particle : particles)
        {
            particle.update(deltaTime, centerX, centerY);
        }

        // Render particles
        window.clear(sf::Color::Black);

        // Draw background stars
        window.draw(stars);

        // Draw orbit path
        window.draw(orbitPath);

        // Draw particles
        for (const auto &particle : particles)
        {
            window.draw(particle.shape);
        }

        window.display();
    }

    return 0;
}
