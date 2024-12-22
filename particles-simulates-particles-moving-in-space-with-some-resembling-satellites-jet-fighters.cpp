#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

// Particle Class: Represents a particle in the simulation
class Particle
{
public:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool isSatellite;

    Particle(float x, float y, float radius, sf::Color color, sf::Vector2f initialVelocity, bool satellite)
        : position(x, y), velocity(initialVelocity), isSatellite(satellite)
    {
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius); // Center the shape
        shape.setPosition(position);
    }

    void update(float deltaTime, float centerX, float centerY)
    {
        if (isSatellite)
        {
            // Circular motion around a point (center of the window)
            float angle = std::atan2(position.y - centerY, position.x - centerX);
            angle += velocity.x * deltaTime;              // Angular velocity
            position.x = centerX + 200 * std::cos(angle); // 200 is orbit radius
            position.y = centerY + 200 * std::sin(angle);
        }
        else
        {
            // Jet fighter motion (zig-zag or random movement)
            position += velocity * deltaTime;

            // Wrap around the screen
            if (position.x < 0)
                position.x = 800;
            if (position.x > 800)
                position.x = 0;
            if (position.y < 0)
                position.y = 600;
            if (position.y > 600)
                position.y = 0;
        }
        shape.setPosition(position);
    }
};

int main()
{
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Space Particle Simulation");
    window.setFramerateLimit(60);

    // Create particles
    std::vector<Particle> particles;
    for (int i = 0; i < 50; ++i)
    {
        float x = std::rand() % 800;
        float y = std::rand() % 600;
        float radius = 5.0f + std::rand() % 5; // Random radius between 5 and 10
        sf::Color color = sf::Color::White;
        sf::Vector2f velocity(
            (std::rand() % 200 - 100) / 100.0f, // Angular velocity for satellites
            (std::rand() % 200 - 100) / 10.0f   // Random velocity for jets
        );

        bool isSatellite = (i < 10); // First 10 particles are satellites
        particles.emplace_back(x, y, radius, color, velocity, isSatellite);
    }

    // Center of the screen (for satellites)
    float centerX = 400.0f;
    float centerY = 300.0f;

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
        for (const auto &particle : particles)
        {
            window.draw(particle.shape);
        }
        window.display();
    }

    return 0;
}
