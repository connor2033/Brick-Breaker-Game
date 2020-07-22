
struct Ball
{

    b2Body* body;
    float speed;
    float angle;

////////////////////BALL/////////////////////////////////
    Ball(b2World &world, float x, float y, float r, b2BodyType type = b2_dynamicBody)
    {
        cout<<"hello"<<endl;


        b2BodyDef bodyDef;
        bodyDef.position.Set(x/pixels_per_meter, y/pixels_per_meter);
        bodyDef.type = type;
        bodyDef.linearDamping = 0;
        b2CircleShape b2shape;

        b2shape.m_radius = r/pixels_per_meter;

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 1;
        fixtureDef.shape = &b2shape;

        body = world.CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);

        sf::CircleShape* shape1 = new sf::CircleShape(r);
        shape1->setOrigin(r,r);
        shape1->setPosition(x, y);
        shape1->setFillColor(sf::Color::Transparent);
        shape1->setOutlineColor(sf::Color::White);
        shape1->setOutlineThickness(-3);

        body->SetUserData(shape1);

        speed = 400;
        angle = 45*PI/180.0;
        body->SetLinearVelocity(b2Vec2(speed*cos(angle)/pixels_per_meter,speed*sin(angle)/pixels_per_meter));

    }

    sf::Vector2f getPosition() {
        sf::Vector2f sfml_pos(body->GetPosition().x*pixels_per_meter, body->GetPosition().y*pixels_per_meter);
        return sfml_pos;
    }

    void setColor(sf::Color c1)
    {
        sf::Shape* shape = static_cast<sf::Shape*>(body->GetUserData());
        shape->setFillColor(c1);
    }


    ////freight train power up
    void setFreightTrain(bool b)
    {
        if (b == true)
        body->GetFixtureList()->SetSensor(true);

        if (b == false)
        body->GetFixtureList()->SetSensor(false);
    }

    void Apply_Force(sf::Vector2f force)
    {
        body->ApplyLinearImpulseToCenter(b2Vec2(force.x, force.y),1);
    }

    void setVelocity(sf::Vector2f vel)
    {
        body->SetLinearVelocity(b2Vec2(vel.x/pixels_per_meter,vel.y/pixels_per_meter));
    }

    void updatePosition()
    {
        b2Vec2 velocity = body->GetLinearVelocity();
        body->SetLinearVelocity((speed/pixels_per_meter)/velocity.Length()*velocity);
    }
    bool isColliding(b2Body* body2)
    {
        for (b2ContactEdge* edge = body->GetContactList(); edge; edge = edge->next) {
            if (edge->other == body2) {
                if (edge->contact->IsTouching()) {
                   return true;
                }
            }
        }
        return false;
    }

    void resetAngle(sf::Vector2f paddle_pos)
    {
        float pb_angle = atan2(this->getPosition().y - paddle_pos.y, this->getPosition().x - paddle_pos.x);
        speed = speed + 7;
        body->SetLinearVelocity(b2Vec2(speed*cos(pb_angle)/pixels_per_meter,speed*sin(pb_angle)/pixels_per_meter));
        //cout<<pb_angle*180/PI<<endl;
    }

    void deleteBall(b2World &world) {
        world.DestroyBody(body);
        body->SetUserData(0);
    }


};

////////////////////////////////////////BRICKS////////////////////////////
struct Brick
{
    b2Body* body;

    Brick(b2World &world, float x, float y, float width, float height, b2BodyType type = b2_dynamicBody)
    {
        b2BodyDef bodyDef;
        bodyDef.position.Set((x + width/2.0)/pixels_per_meter, (y + height/2.0)/pixels_per_meter);
        bodyDef.type = type;
        bodyDef.linearDamping = 0;
        b2PolygonShape b2shape;
        b2shape.SetAsBox(width/pixels_per_meter/2.0, height/pixels_per_meter/2.0);
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 1;
        fixtureDef.shape = &b2shape;

        body = world.CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);

        sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(width, height));
        shape->setOrigin(width/2.0, height/2.0);
        shape->setPosition(x, y);
        shape->setFillColor(sf::Color::Transparent);
        shape->setOutlineColor(sf::Color::White);
        shape->setOutlineThickness(-3);

        body->SetUserData(shape);
    }

    bool isColliding(Ball b)
    {
        for (b2ContactEdge* edge = body->GetContactList(); edge; edge = edge->next) {
            if (edge->other == b.body) {
                if (edge->contact->IsTouching()) {
                   return true;
                }
            }
        }
        return false;
    }

    void deleteBody(b2World &world)
    {
        world.DestroyBody(body);
        body->SetUserData(0);
    }


};


//////////////////////////////////PADDLE////////////////////////////

 struct Paddle
{
    b2Body* body;
    sf::Vector2f pos;
    float speed;
    float p_width;
    float p_height;


    Paddle(b2World &world, float x, float y, float width, float height, b2BodyType type = b2_kinematicBody)
    {
        p_height = height;
        p_width = width;
        speed = 500.0;
        b2BodyDef bodyDef;
        bodyDef.position.Set((x + width/2.0)/pixels_per_meter, (y + p_height/2.0)/pixels_per_meter);
        bodyDef.type = type;
        bodyDef.linearDamping = 0;
        b2PolygonShape b2shape;
        b2shape.SetAsBox(width/pixels_per_meter/2.0, height/pixels_per_meter/2.0);
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 1;
        fixtureDef.shape = &b2shape;

        body = world.CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);

        sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(width, height));
        shape->setOrigin(width/2.0, height/2.0);
        shape->setPosition(x, y);
        shape->setFillColor(sf::Color::White);

        body->SetUserData(shape);
    }

    sf::Vector2f getPosition() {
        sf::Vector2f sfml_pos(body->GetPosition().x*pixels_per_meter, body->GetPosition().y*pixels_per_meter);
        return sfml_pos;
    }


    void updatePosition()
        {
            pos = sf::Vector2f (body->GetPosition().x*pixels_per_meter, body->GetPosition().y*pixels_per_meter);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                body->SetLinearVelocity(b2Vec2(-speed/pixels_per_meter, 0));
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                body->SetLinearVelocity(b2Vec2(speed/pixels_per_meter, 0));
            }
            else
            {
                body->SetLinearVelocity((b2Vec2(0,0)));
            }

        }

    ////super speed power up
    void setSuperSpeed(bool b)
    {
        if (b == true)
        speed = 1500;

        //(default)
        if (b == false)
        speed = 500;
    }

    void deletePaddle(b2World &world) {
        world.DestroyBody(body);
        body->SetUserData(0);
    }


    void resetSize(b2World &world, float p_size)
    {
        float x = body->GetPosition().x;
        float y = body->GetPosition().y;
        //cout<<x<<endl;
        world.DestroyBody(body);
        body->SetUserData(0);
        b2BodyDef bodyDef;
        bodyDef.position.Set(x, y);
        bodyDef.type = b2_kinematicBody;
        bodyDef.linearDamping = 0;
        b2PolygonShape b2shape;
        b2shape.SetAsBox(p_size*p_width/pixels_per_meter/2.0, p_height/pixels_per_meter/2.0);
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 1;
        fixtureDef.shape = &b2shape;

        body = world.CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);

        sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(p_size*p_width, p_height));
        shape->setOrigin(p_size*p_width/2.0, p_height/2.0);
        shape->setPosition(x*pixels_per_meter, y*pixels_per_meter);
        shape->setFillColor(sf::Color::White);

        body->SetUserData(shape);
    }

};


