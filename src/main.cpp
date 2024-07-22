#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include <raylib-cpp.hpp>

int main()
{
    int screenWidth = 1280;
    int screenHeight = 720;
    SetTargetFPS(0);

    raylib::Window window(screenWidth, screenHeight, "Fluffy Tree!");

    raylib::Color ground_color(0xbfd0b9ff);

    raylib::Model tree("res/tree.glb");
    raylib::Vector3 tree_pos = {0, 0, 0};
    raylib::Camera3D camera({0, 3, 10}, {0, 3, 0}, {0, 1, 0}, 60, CAMERA_PERSPECTIVE);

    raylib::Shader trunk_shader(nullptr, "res/trunk.fs");
    raylib::Material trunk_mat;
    trunk_mat.SetShader(trunk_shader);

    raylib::Texture2D leaf_tex("res/foliage_alpha3.png");
    leaf_tex.SetFilter(TEXTURE_FILTER_ANISOTROPIC_16X | TEXTURE_FILTER_TRILINEAR);
    raylib::Shader leaf_shader("res/leaf.vs", "res/leaf.fs");
    raylib::Material leaf_mat;
    leaf_mat.SetShader(leaf_shader);
    leaf_mat.SetTexture(MATERIAL_MAP_DIFFUSE, leaf_tex);
    int modelViewMatrix = leaf_shader.GetLocation("modelViewMatrix");


    while (!window.ShouldClose())
    {
        camera.Update(CAMERA_ORBITAL);

        BeginDrawing();
        {
            window.ClearBackground(BLACK);

            camera.BeginMode();
            {
                DrawCylinder(tree_pos, 5, 5, 0.01f, 50, ground_color);
                if (tree.IsReady())
                {
                    raylib::Matrix transform = raylib::Matrix::Identity();
                    DrawMesh(tree.meshes[0], trunk_mat, transform);

                    raylib::Matrix modelView = MatrixMultiply(GetCameraMatrix(camera), tree.transform);

                    leaf_shader.SetValue(modelViewMatrix, modelView);
                    BeginBlendMode(BLEND_ALPHA);
                    {
                        DrawMesh(tree.meshes[1], leaf_mat, transform);
                    }
                    EndBlendMode();
                }
            }
            camera.EndMode();

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    // Workaround to make it not crash
    trunk_mat.maps = nullptr;
    leaf_mat.maps = nullptr;

    return 0;
}