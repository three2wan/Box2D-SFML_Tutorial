#include<SFML\Graphics.hpp>
#include<Box2D\Box2D.h>

static const float scale = 32.0f;

void CreateGround(b2World& world, float x, float y);

void CreateBomb(b2World& world, int mouseX, int mouseY);


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Box2D and SFML Tutorial");
	window.setFramerateLimit(60);

	b2Vec2 gravity(0, 9.8f);
	b2World world(gravity);
	CreateGround(world, 400, 500);

	sf::Texture groundTex;
	sf::Texture bombTex;
	groundTex.loadFromFile("Resources/ground.png");
	bombTex.loadFromFile("Resources/Spikey.png");

	while (window.isOpen()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int mouseX = sf::Mouse::getPosition(window).x;
			int mouseY = sf::Mouse::getPosition(window).y;
			CreateBomb(world, mouseX, mouseY);
		}

		world.Step(1 / 60.f, 8, 3);

		window.clear(sf::Color::White);
		int bodyCount = 0;
		for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext()) {
			if (bodyIterator->GetType() == b2_dynamicBody) {
				sf::Sprite sprite;
				sprite.setTexture(bombTex);
				sprite.setOrigin((33/2), (33 / 2));
				sprite.setPosition(scale * bodyIterator->GetPosition().x, scale * bodyIterator->GetPosition().y);
				sprite.setRotation(bodyIterator->GetAngle() * 180 / b2_pi);
				window.draw(sprite);
				++bodyCount;
			}
			else {
				sf::Sprite groundSpt;
				groundSpt.setTexture(groundTex);
				groundSpt.setOrigin(400, 8);
				groundSpt.setPosition(scale * bodyIterator->GetPosition().x, scale * bodyIterator->GetPosition().y);
				groundSpt.setRotation(bodyIterator->GetAngle() * 180 / b2_pi);
				window.draw(groundSpt);
			}
		}
		window.display();
	}
	return 0;
}

void CreateGround(b2World& world, float x, float y) {
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(x / scale, y / scale);
	bodyDef.type = b2_staticBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((800 / 2) / scale, (16 / 2) / scale);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}

void CreateBomb(b2World& world, int mouseX, int mouseY) {
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(mouseX / scale, mouseY / scale);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_p.Set(0, 0);
	shape.m_radius = ((33 / 2) / scale);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;
	fixtureDef.friction = 0.7f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}
