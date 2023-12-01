//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef pointLight_h
#define pointLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class PointLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    int lightNumber;

    PointLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        lightNumber = num;
    }
    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();

        if (lightNumber == 1) {
            lightingShader.setVec3("pointLights[0].position", position);
            lightingShader.setVec3("pointLights[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[0].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[0].k_c", k_c);
            lightingShader.setFloat("pointLights[0].k_l", k_l);
            lightingShader.setFloat("pointLights[0].k_q", k_q);
        }

        else if (lightNumber == 2)
        {
            lightingShader.setVec3("pointLights[1].position", position);
            lightingShader.setVec3("pointLights[1].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[1].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[1].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[1].k_c", k_c);
            lightingShader.setFloat("pointLights[1].k_l", k_l);
            lightingShader.setFloat("pointLights[1].k_q", k_q);
        }
        else if (lightNumber == 3)
        {
            lightingShader.setVec3("pointLights[2].position", position);
            lightingShader.setVec3("pointLights[2].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[2].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[2].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[2].k_c", k_c);
            lightingShader.setFloat("pointLights[2].k_l", k_l);
            lightingShader.setFloat("pointLights[2].k_q", k_q);
        }
        else if (lightNumber == 4)
        {
            lightingShader.setVec3("pointLights[3].position", position);
            lightingShader.setVec3("pointLights[3].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[3].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[3].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[3].k_c", k_c);
            lightingShader.setFloat("pointLights[3].k_l", k_l);
            lightingShader.setFloat("pointLights[3].k_q", k_q);

        }
        else if (lightNumber == 5)
        {
            lightingShader.setVec3("pointLights[4].position", position);
            lightingShader.setVec3("pointLights[4].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[4].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[4].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[4].k_c", k_c);
            lightingShader.setFloat("pointLights[4].k_l", k_l);
            lightingShader.setFloat("pointLights[4].k_q", k_q);

        }
        else if (lightNumber == 6)
        {
            lightingShader.setVec3("pointLights[5].position", position);
            lightingShader.setVec3("pointLights[5].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[5].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[5].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[5].k_c", k_c);
            lightingShader.setFloat("pointLights[5].k_l", k_l);
            lightingShader.setFloat("pointLights[5].k_q", k_q);

        }
        else if (lightNumber == 7)
        {
            lightingShader.setVec3("pointLights[6].position", position);
            lightingShader.setVec3("pointLights[6].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[6].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[6].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[6].k_c", k_c);
            lightingShader.setFloat("pointLights[6].k_l", k_l);
            lightingShader.setFloat("pointLights[6].k_q", k_q);

        }
        else if (lightNumber == 8)
        {
            lightingShader.setVec3("pointLights[7].position", position);
            lightingShader.setVec3("pointLights[7].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[7].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[7].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[7].k_c", k_c);
            lightingShader.setFloat("pointLights[7].k_l", k_l);
            lightingShader.setFloat("pointLights[7].k_q", k_q);
        }
        else if (lightNumber == 9)
        {
            lightingShader.setVec3("pointLights[8].position", position);
            lightingShader.setVec3("pointLights[8].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[8].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[8].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[8].k_c", k_c);
            lightingShader.setFloat("pointLights[8].k_l", k_l);
            lightingShader.setFloat("pointLights[8].k_q", k_q);

        }
        else if (lightNumber == 10)
        {
            lightingShader.setVec3("pointLights[9].position", position);
            lightingShader.setVec3("pointLights[9].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[9].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[9].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[9].k_c", k_c);
            lightingShader.setFloat("pointLights[9].k_l", k_l);
            lightingShader.setFloat("pointLights[9].k_q", k_q);

        }
        else if (lightNumber == 11)
        {
            lightingShader.setVec3("pointLights[10].position", position);
            lightingShader.setVec3("pointLights[10].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[10].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[10].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[10].k_c", k_c);
            lightingShader.setFloat("pointLights[10].k_l", k_l);
            lightingShader.setFloat("pointLights[10].k_q", k_q);

        }
        else if (lightNumber == 12)
        {
            lightingShader.setVec3("pointLights[11].position", position);
            lightingShader.setVec3("pointLights[11].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[11].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[11].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[11].k_c", k_c);
            lightingShader.setFloat("pointLights[11].k_l", k_l);
            lightingShader.setFloat("pointLights[11].k_q", k_q);

        }
        else if (lightNumber == 13)
        {
            lightingShader.setVec3("pointLights[12].position", position);
            lightingShader.setVec3("pointLights[12].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[12].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[12].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[12].k_c", k_c);
            lightingShader.setFloat("pointLights[12].k_l", k_l);
            lightingShader.setFloat("pointLights[12].k_q", k_q);


        }

        else if (lightNumber == 14)
        {
            lightingShader.setVec3("pointLights[13].position", position);
            lightingShader.setVec3("pointLights[13].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[13].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[13].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[13].k_c", k_c);
            lightingShader.setFloat("pointLights[13].k_l", k_l);
            lightingShader.setFloat("pointLights[13].k_q", k_q);


        }

        else if (lightNumber == 15)
        {
            lightingShader.setVec3("pointLights[14].position", position);
            lightingShader.setVec3("pointLights[14].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[14].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[14].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[14].k_c", k_c);
            lightingShader.setFloat("pointLights[14].k_l", k_l);
            lightingShader.setFloat("pointLights[14].k_q", k_q);


        }

        else if (lightNumber == 16)
        {
            lightingShader.setVec3("pointLights[15].position", position);
            lightingShader.setVec3("pointLights[15].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[15].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[15].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[15].k_c", k_c);
            lightingShader.setFloat("pointLights[15].k_l", k_l);
            lightingShader.setFloat("pointLights[15].k_q", k_q);

        }
        else if (lightNumber == 17)
        {
            lightingShader.setVec3("pointLights[16].position", position);
            lightingShader.setVec3("pointLights[16].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[16].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[16].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[16].k_c", k_c);
            lightingShader.setFloat("pointLights[16].k_l", k_l);
            lightingShader.setFloat("pointLights[16].k_q", k_q);

        }
        else if (lightNumber == 18)
        {
            lightingShader.setVec3("pointLights[17].position", position);
            lightingShader.setVec3("pointLights[17].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[17].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[17].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[17].k_c", k_c);
            lightingShader.setFloat("pointLights[17].k_l", k_l);
            lightingShader.setFloat("pointLights[17].k_q", k_q);


        }
        else if (lightNumber == 19)
        {
            lightingShader.setVec3("pointLights[18].position", position);
            lightingShader.setVec3("pointLights[18].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[18].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[18].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[18].k_c", k_c);
            lightingShader.setFloat("pointLights[18].k_l", k_l);
            lightingShader.setFloat("pointLights[18].k_q", k_q);

        }
        else if (lightNumber == 20)
        {
            lightingShader.setVec3("pointLights[19].position", position);
            lightingShader.setVec3("pointLights[19].ambient", ambientOn* ambient);
            lightingShader.setVec3("pointLights[19].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("pointLights[19].specular", specularOn* specular);
            lightingShader.setFloat("pointLights[19].k_c", k_c);
            lightingShader.setFloat("pointLights[19].k_l", k_l);
            lightingShader.setFloat("pointLights[19].k_q", k_q);


        }




    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* pointLight_h */
