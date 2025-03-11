#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

/*
    Hi! I'm Elvis and I got an idea during my vacations so I'll be working on this project for a while
    I'm a System Engineering student and I'm always looking for new projects to work on, so I decided to create this one, just to work on my skills and continue enjoying programming
    I'm also working on my documentation skills, so I'll be documenting this project as much as I can

    This projects has different objectives and future improvements: 
    1. To simulate the gravitational interaction between two bodies
    2. To simulate the gravitational interaction between multiples bodies
    3. To simulate the gravitational interaction between bodies and a central body
    4. To simulate the gravitational interaction between bodies and a central body with a fixed orbit
    5. To simulate the gravitational interaction between bodies and a central body with a fixed orbit and a fixed rotation (and more objectives likes this...)
    6. To simulate collisions between bodies
    7. To simulate the different gravitational interaction between bodies with different masses, shapes and other physic effects

    The project will be divided in different parts, each part will be a different version of the simulation with different objectives and improvements
    This was mainly created to learn more about physics and simulations, and to improve my programming skills and to have fun with it

    Also, I'm doing my research on the topic, and OpenGL took my attention so probably I'll try the same simulation using OpenGL in the future and others techniques and technologies
*/

/*
    This a simple gravity simulation using SFML Library. 
    The simulation uses Newton's Law of Universal Gravitation: F = G * (m1 * m2) / r^2
    where F is the force between two objects, G is the gravitational constant, m1 and m2 are the masses of the objects and r is the distance between them
*/
const float G = 6.67430e-11f;

/*
    Simulations tend to go faster than real life, so we need to slow it down to see what's happening
    This constant is the time step between each frame of the simulation, I saw 0.1 is a recommended value so I'll use it
*/
const float TIME_STEP = 0.1f;

/*
    This structure represents a body in the simulation
    It has a mass, position, velocity, and a shape representing it
    
    I'm using a struct due to various advantages like: 
    1. It's a simple data structure that groups variables
    2. It's a value type
    3. It's public by default so we can manipulate multiples objects in list or arrays, perfect for bodies (like star, particles...)
    
    And also using separate variables would be a hell to manage, so I'll use a struct to group them, but a class could be used too but in a different way that I'll probably test and compare with this one
    By the moment, I'm more comfortable with structs due to my actual knowledge and experience
*/
struct Body
{
    /*
        Properties of the body! position, velocity, mass and shape, very basic properties of a body

        These vectors are from the SFML library, they represent a 2D vector with x and y components
        The position is the position of the body in the window
        The velocity is the velocity of the body something like (-1, 2) means the body is moving to the left and up, it's also x and y components
        The mass is the mass of the body, it's a float value
        The shape is the shape of the body, bodies will be circles 
    */
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    sf::CircleShape shape;

    /*
        Constructor of the body, it initializes the properties of the body
        It receives the x and y position, the mass, the radius and the color of the body
        Velocity it's initialized as (0, 0) because the body is not moving at the beginning
    */
    Body(float x, float y, float mass, float radius, sf::Color color)
    {
        position = {x, y};
        this->mass = mass;
        velocity = {0.0f, 0.0f};
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);
    }
};


/*
    This functions calculate the gravitational force between two or more bodies in the simulation
    It receives a vector of bodies and calculates the force between them
    The force is calculated using Newton's Law of Universal Gravitation previously mentioned
*/
void gravity(std::vector<Body>& bodies)
{
    for (size_t i = 0; i < bodies.size(); i++)
    {
        sf::Vector2f force = {0.0f, 0.0f}; // For each body the force is initialized as (0, 0)
        for (size_t j = 0; j < bodies.size(); j++)
        {
            if (i == j)
            {
                // i is one body and j is other, so it's comparing itself to others, not itself, this line skips the comparison with itself
                continue;
            }

            // This vector contains x and y components of the direction between the body j and the body i 
            sf::Vector2f direction = bodies[j].position - bodies[i].position;

            // Distance with pythagoras theorem, how far are the bodies from each other
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            
            // There is a thing with this formula, if the distance is 0, the force will be infinite, and also low distances will cause high forces, we should try avoid this
            // So we need to add a minimum distance to avoid this, I'll use a minium distance, it can be changed to study the effects of this
            if (distance < 5.0f)
            {
                continue;
            }

            // Now we calculate the force between the bodies using Newton's Law
            float gravityForce = G * (bodies[i].mass * bodies[j].mass) / (distance * distance);

            // Normalizing the direction vector, it's a vector with the same direction but with a magnitude of 1
            sf::Vector2f normalizedDirection = direction / distance;

            // the force is the normalized direction multiplied by the gravity force, now we have a vector with the direction and magnitude of the force between the bodies
            // this force is applied to the body i
            force += normalizedDirection * gravityForce;
        }
        // Now the acceleration, these are basic formula of physics, Newton's Laws
        // F = m * a, so a = F / m and velocity = v + a * t
        sf::Vector2f acceleration = force / bodies[i].mass;
        bodies[i].velocity += acceleration * TIME_STEP;
    }
}

// The bodies are moving due to their velocity, their position needs to be updated
// Note: recently I discovered the advantages of reference and pointers, would say they're perfect for this task, I'll use reference in the parameters
void bodiesMovement(std::vector<Body>& bodies)
{
    for (auto& body : bodies)
    {
        // We update the position of the body using the velocity
        body.position += body.velocity * TIME_STEP;
        body.shape.setPosition(body.position);
    }
}

// Now we are in the main function, the entry point of the program, here we only have to use the functions we created
int main() 
{
    // Starting the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gravity Simulation");

    // The bodies in the simulation
    std::vector<Body> bodies = {
        // The sun
        Body(400, 300, 5e10f, 10.0f, sf::Color::Yellow),
        // The earth
        Body(300, 300, 1e10f, 5.0f, sf::Color::Blue),
        // The mars
        Body(200, 300, 1e10f, 5.0f, sf::Color::Red)
    }; 

    // this loop will run until the window is closed 
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

        // We apply the functions to the bodies
        gravity(bodies);
        bodiesMovement(bodies);

        // With this we clear the window and draw the bodies in every iteration
        window.clear();
        for (auto& body : bodies)
        {
            window.draw(body.shape);
        }
        window.display();
    }
    // Now we should run and test the program!
    return 0;
}

