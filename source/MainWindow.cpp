#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <functional>

#include "WindowCallback.h"
#include "HedcutRenderer.h"

// Window settings
const unsigned int SCR_WIDTH = 2560;
const unsigned int SCR_HEIGHT = 1440;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // Init GLFW Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    GLFWwindow* pWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hedcut 3D", NULL, NULL);
    if (pWindow == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(pWindow);
    
    // GLFW callbacks
    WindowCallback windowCallbackInstance();
    WindowCallback::SetGLFWCallbacks(pWindow);

    // Load OpenGL Functions with GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Failed to init GLEW\n";
    }

    // Global opengl state
    //z buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    /*
    glEnable(GL_MULTISAMPLE);
    */

    // IMGui init    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460");


    // Load Meshes
    std::vector<Mesh> meshList;

    models.push_back(Model(".\\models\\stanford-bunny.obj"));
    models.push_back(Model(".\\models\\max-planck.obj"));
    models.push_back(Model(".\\models\\lucy.obj"));
    models.push_back(Model(".\\models\\rapid.obj"));
    models.push_back(Model(".\\models\\horse.obj"));
    //models.push_back(Model(".\\models\\Nefertiti.obj"));

    models.push_back(Model(".\\models\\torus.obj"));
    models.push_back(Model(".\\models\\knot.obj"));
    models.push_back(Model(".\\models\\xyzrgb_dragon.obj"));

    Model* currentModel = &models[0];

    //Load Shaders
    GLuint diffuse = loadShader(".\\shaders\\diffuse.vert", ".\\shaders\\diffuse.frag");
    GLuint base = loadShader(".\\shaders\\base.vert", ".\\shaders\\base.frag");
    GLuint apparentRidges = loadShader(".\\shaders\\apparentRidges.vert", ".\\shaders\\apparentRidges.frag", ".\\shaders\\apparentRidges.geom");
    //GLuint apparentRidges = diffuse;
    GLuint maxPDShader = loadShader(".\\shaders\\PDmax.vert", ".\\shaders\\PDmax.frag", ".\\shaders\\PDmax.geom");
    GLuint minPDShader = loadShader(".\\shaders\\PDmin.vert", ".\\shaders\\PDmin.frag", ".\\shaders\\PDmin.geom");
    GLuint curvHeat = loadShader(".\\shaders\\curvatureHeat.vert", ".\\shaders\\curvatureHeat.frag");
    GLuint visualizeViewDep = loadShader(".\\shaders\\visualizeViewDep.vert", ".\\shaders\\visualizeViewDep.frag", ".\\shaders\\visualizeViewDep.geom");
    GLuint visualizeDt1q1 = loadShader(".\\shaders\\visualizeDt1q1.vert", ".\\shaders\\visualizeDt1q1.frag", ".\\shaders\\visualizeDt1q1.geom");
    GLuint normalsShader = loadShader(".\\shaders\\normals.vert", ".\\shaders\\normals.frag", ".\\shaders\\normals.geom");


    //GLuint* currentShader = &apparantRidges;
    GLuint* currentShader = &diffuse;

    //view
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    //light settings
    glm::vec3 lightPosInit = glm::vec3(-1.0f, 1.0f, 1.5f);
    glm::vec3 lightPos = lightPosInit;
    glm::vec3 lightDiffuse = glm::vec3(1, 1, 1);
    glm::vec3 lightSpecular = glm::vec3(1, 1, 1);


    int renderModeSelect = 0;
    //render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //yDegrees += 1;
        //yDegrees =int(yDegrees)%360;

        glClearColor(background.x, background.y, background.z, 0.0); //background

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLineWidth(lineWidth);
        //IMGui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //IMGui window
        ImGui::Begin("Apparent Ridges");

        ImGui::Columns(2, nullptr, false);

        ImGui::RadioButton("Line Drawing with Apparent Ridges", &renderModeSelect, 0);
        ImGui::RadioButton("Curvature Heatmap", &renderModeSelect, 1);
        ImGui::RadioButton("Diffuse", &renderModeSelect, 2);

        ImGui::NextColumn();
        ImGui::RadioButton("View Dependent Curvature", &renderModeSelect, 3);
        ImGui::RadioButton("Derivative of View Dependent Curvature", &renderModeSelect, 4);

        ImGui::NextColumn();
        ImGui::Checkbox("Draw Faded Lines", &drawFaded);
        ImGui::Checkbox("Cull Apparent Ridges", &apparentCullFaces);
        ImGui::Checkbox("Transparent", &transparent);

        ImGui::NextColumn();
        ImGui::Checkbox("Principal Directions", &PDsOn);
        ImGui::Checkbox("Normals", &normalsOn);
        //ImGui::Checkbox("View Dependent Curvature Direction", &viewDepPDOn);

        ImGui::Columns(1);
        ImGui::Text("Click + drag to move model. Ctrl + click + drag to move light source. Scroll to zoom.");
        const char* listboxItems[] = { "Bunny", "Planck","Lucy", "David", "Brain", /*"Nefertiti",*/"Horse","Torus","Knot","Cow","Dragon" };
        static int currentlistboxItem = 0;
        ImGui::ListBox("Model", &currentlistboxItem, listboxItems, IM_ARRAYSIZE(listboxItems), 3);
        currentModel = &models[currentlistboxItem];

        ImGui::SliderFloat("Line Width", &lineWidth, 0.1f, 10.0f);
        ImGui::SliderFloat("Threshold", &thresholdScale, 0.0f, 5.0f);
        ImGui::SliderFloat("Principal Directions Arrow Length", &PDLengthScale, 0.0f, 0.8f);
        ImGuiColorEditFlags misc_flags = (0 | ImGuiColorEditFlags_NoDragDrop | 0 | ImGuiColorEditFlags_NoOptions);
        ImGui::ColorEdit3("Line Color", (float*)&lineColor, misc_flags);
        ImGui::ColorEdit3("Background Color", (float*)&background, misc_flags);
        //ImGui::SliderFloat("Rotate Global Light Source", &lightDegrees, 0.0f, 360.0f);   
        //ImGui::SliderFloat("Brightness", &diffuse, 0.0f, 2.0f);
        ImGui::End();


        if (renderModeSelect == 0) { currentShader = &apparentRidges; currentModel->apparentRidges = true; }
        else if (renderModeSelect == 1) { currentShader = &curvHeat; currentModel->apparentRidges = false; }
        else if (renderModeSelect == 2) { currentShader = &diffuse; currentModel->apparentRidges = false; }
        else if (renderModeSelect == 3) { currentShader = &visualizeViewDep; currentModel->apparentRidges = true; }
        else if (renderModeSelect == 4) { currentShader = &visualizeDt1q1; currentModel->apparentRidges = true; }


        glUseProgram(*currentShader);

        //Uniforms
        glm::mat4 lightRotate = glm::rotate(glm::mat4(1), glm::radians(lightDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
        lightPos = glm::vec3(lightRotate * glm::vec4(lightPosInit, 0.0f));

        glUniform3f(glGetUniformLocation(*currentShader, "light.position"), lightPos.x, lightPos.y, lightPos.z);
        //glUniform3f(glGetUniformLocation(*currentShader, "light.diffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);

        //opengl matrice transforms are applied from the right side first. (last first)
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f)); //move in world space
        model *= scrollModel; //scale
        model = glm::scale(model, glm::vec3(currentModel->modelScaleFactor));
        model = model * mouseRotation;
        model = glm::translate(model, (-1.0f * currentModel->center)); //bring to origin
        glm::mat4 view = glm::lookAt(cameraPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        currentModel->modelMatrix = model;
        currentModel->viewPos = cameraPos;

        glUniformMatrix4fv(glGetUniformLocation(*currentShader, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(*currentShader, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(*currentShader, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniform3f(glGetUniformLocation(*currentShader, "viewPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
        if (renderModeSelect == 0 || renderModeSelect == 3 || renderModeSelect == 4) {
            if (!transparent) {
                //render base model
                glUseProgram(base);
                glUniformMatrix4fv(glGetUniformLocation(base, "model"), 1, GL_FALSE, &model[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(base, "view"), 1, GL_FALSE, &view[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(base, "projection"), 1, GL_FALSE, &projection[0][0]);
                glUniform3f(glGetUniformLocation(base, "backgroundColor"), background.x, background.y, background.z);
                currentModel->render(base);
            }

            //render apparent ridges
            currentModel->apparentRidges = true;
            glUseProgram(currentModel->viewDepCurvatureCompute);
            //glUniform3f(glGetUniformLocation(currentModel->viewDepCurvatureCompute, "viewPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
            //glUniformMatrix4fv(glGetUniformLocation(currentModel->viewDepCurvatureCompute, "model"), 1, GL_FALSE, &model[0][0]);

            glUseProgram(currentModel->Dt1q1Compute);
            glUniform3f(glGetUniformLocation(currentModel->Dt1q1Compute, "viewPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
            //glUniformMatrix4fv(glGetUniformLocation(currentModel->Dt1q1Compute, "model"), 1, GL_FALSE, &model[0][0]);

            glUseProgram(*currentShader);
            //threshold is scaled to the reciprocal of feature size

            //if (currentModel->minDistance>1.0f)
                //glUniform1f(glGetUniformLocation(apparentRidges,"threshold"),0.2f*thresholdScale/(currentModel->minDistance));
            glUniform1f(glGetUniformLocation(*currentShader, "threshold"), 0.02f * thresholdScale / (currentModel->minDistance * currentModel->minDistance));

            //glUniform1f(glGetUniformLocation(apparentRidges, "threshold"), std::numeric_limits<float>::lowest());
        //else
          //  glUniform1f(glGetUniformLocation(apparentRidges, "threshold"), 3.0f * thresholdScale * currentModel->minDistance);

            glUniform3f(glGetUniformLocation(*currentShader, "lineColor"), lineColor.x, lineColor.y, lineColor.z);
            glUniform3f(glGetUniformLocation(*currentShader, "backgroundColor"), background.x, background.y, background.z);
            glUniform1i(glGetUniformLocation(*currentShader, "drawFaded"), drawFaded);
            glUniform1i(glGetUniformLocation(*currentShader, "cull"), apparentCullFaces);

            currentModel->apparentRidges = false;
        }

        if (PDsOn) {
            glLineWidth(2);
            //Render Principal Directions
            glUseProgram(maxPDShader);
            glUniform1f(glGetUniformLocation(maxPDShader, "magnitude"), 0.01f * PDLengthScale * scrollModel[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(maxPDShader, "model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(maxPDShader, "view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(maxPDShader, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform1ui(glGetUniformLocation(maxPDShader, "size"), currentModel->vertices.size());

            currentModel->render(maxPDShader);

            glUseProgram(minPDShader);
            glUniform1f(glGetUniformLocation(minPDShader, "magnitude"), 0.01f * PDLengthScale * scrollModel[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(minPDShader, "model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(minPDShader, "view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(minPDShader, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform1ui(glGetUniformLocation(minPDShader, "size"), currentModel->vertices.size());

            currentModel->render(minPDShader);
            glLineWidth(lineWidth);
        }
        if (normalsOn) {
            glLineWidth(2);
            //Render Principal Directions
            glUseProgram(normalsShader);
            glUniform1f(glGetUniformLocation(normalsShader, "magnitude"), 0.015f * PDLengthScale * scrollModel[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(normalsShader, "model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(normalsShader, "view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(normalsShader, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform1ui(glGetUniformLocation(normalsShader, "size"), currentModel->vertices.size());

            currentModel->render(normalsShader);

            glUseProgram(normalsShader);
            glUniform1f(glGetUniformLocation(normalsShader, "magnitude"), 0.015f * PDLengthScale * scrollModel[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(normalsShader, "model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(normalsShader, "view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(normalsShader, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform1ui(glGetUniformLocation(normalsShader, "size"), currentModel->vertices.size());

            currentModel->render(normalsShader);
            glLineWidth(lineWidth);
        }
        if (viewDepPDOn) {
            /*
                glUseProgram(viewDepPDShader);
                glUniform1f(glGetUniformLocation(viewDepPDShader, "magnitude"), 0.01f * PDLengthScale * scrollModel[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(viewDepPDShader, "model"), 1, GL_FALSE, &model[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(viewDepPDShader, "view"), 1, GL_FALSE, &view[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(viewDepPDShader, "projection"), 1, GL_FALSE, &projection[0][0]);
                glUniform1ui(glGetUniformLocation(viewDepPDShader, "size"), currentModel->vertices.size());

                currentModel->render(viewDepPDShader);
            */
        }

        currentModel->render(*currentShader);

        glUseProgram(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete ImGUI instances
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}


