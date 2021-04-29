#pragma once

 #include"../CreatureEngine/Creature_Engine.h"





//extern Camera3D *Camera_3D;
//extern iVec2 GLOBAL_OldMouse;
//extern iVec2 GLOBAL_Delta_Mouse;
extern bool Drag_Triggered;

/* This Camera Situation is all wrong, figure a way to fix this */
//  #define CAMERA_SPEED 4.0f

//#define ZOOM_SPEED 1.0
const float ZOOM_SPEED = 1.0;

//  
//  
//  Listener KeyListener([](Event _msg)
//  {
//      DEBUG_CODE(Print("Key Entered: " << _msg.wParam));
//      switch (_msg.wParam)
//      {
//      case 37:
//      {// Left Key
//          Application::getCamera().MoveX(-CAMERA_SPEED);
//          Camera_3D->MoveLeft(CAMERA_SPEED);
//      }
//      break;
//  
//      case 38:
//      {// Up  Key
//          Application::getCamera().MoveY(-CAMERA_SPEED);
//          Camera_3D->MoveForward(CAMERA_SPEED);
//      }
//      break;
//  
//      case 39:
//      {// Right  Key
//          Application::getCamera().MoveX(CAMERA_SPEED);
//          Camera_3D->MoveRight(CAMERA_SPEED);
//      }
//      break;
//  
//      case 40:
//      {// Down  Key
//          Application::getCamera().MoveY(CAMERA_SPEED);
//          Camera_3D->MoveBack(CAMERA_SPEED);
//      }
//      break;
//  
//      case 189:
//      {//- Key
//          Application::getCamera().ZoomOut(ZOOM_SPEED);
//      }break;
//  
//      case 187:
//      {//+ Key
//          Application::getCamera().ZoomIn(ZOOM_SPEED);
//      }break;
//  
//  
//  
//      case 188:
//      {// <  
//         // static float keyRotate = Application::getCamera().g_Rotation();
//  
//          float R = (float)(DEGREES(Application::getCamera().g_Rotation()));
//          Application::getCamera().s_Rotation(R - 20);// (float)(RADIANS(Rot)));
//          Camera_3D->RotateY((float)-RADIANS(CAMERA_SPEED));
//  
//          // Application::getCamera().Rotate(-1);
//      }
//      break;
//  
//      case 190:
//      {// > 
//          Application::getCamera().Rotate(2);
//          Camera_3D->RotateY((float)RADIANS(CAMERA_SPEED));
//      }
//      break;
//  
//  
//  
//  
//      }// End of Switch
//  
//  });
//  Listener MouseWheel([](Event _msg)
//  {
//      Vec2 Mpos = SplitLParam((int)_msg.lParam);
//  
//      if ((int16_t)SplitLParam((int)_msg.wParam).y > 0)
//      {// Mouse Wheel UP
//          Application::getCamera().ZoomInto(Mpos, ZOOM_SPEED);
//          Camera_3D->MoveForward(CAMERA_SPEED);
//  
//      }
//      else
//      {// Mouse Wheel DOWN
//          Application::getCamera().ZoomOutFrom(Mpos, ZOOM_SPEED);
//          Camera_3D->MoveBack(CAMERA_SPEED);
//  
//      }
//  });
//  Listener Mouse_Click([](Event _msg)
//  {
//      iVec2 NewMouse = SplitLParam((int)_msg.lParam);
//  
//      POINT Mouse_Position = { NewMouse.x, NewMouse.y };
//      if (DragDetect(Application::getWindow().g_Handle(), Mouse_Position))
//      {// We are Dragging the Screen, Get the Normal and the Velocity and apply to the Rotation of the 3D Camera
//         // GLOBAL_Delta_Mouse = NewMouse - GLOBAL_OldMouse;
//         // GLOBAL_OldMouse = NewMouse;
//  
//          Drag_Triggered = true;
//          // Print( " Dragging the Mouse :" << NewMouse << " : " << GLOBAL_Delta_Mouse);
//      }
//  });
 











struct guiLayer
    : Layer
{
    Vec2  Camera_Position{ 0,0 };
    float Rot{ 0 };

    Camera3D *Camera_3D{};

    void OnRender() override
    {
        // Start the Camera Widget 
        ImGui::Begin("Camera");
        {

            ImGui::SetWindowPos(ImVec2(Application::getWindow().Width() - (Application::getWindow().Height() * 0.15f), 0), true);
            ImGui::SetWindowSize
            (
                {
                    Application::getWindow().Width() * 0.15f,
                    Application::getWindow().Height() - ((Application::getWindow().Height()  * 0.2f))
                }
            );

            float
                H = (float)Application::getCamera().Height(),
                W = (float)Application::getCamera().Width();

            float Z = (float)Application::getCamera().g_Zoom();


            // Position Slider for the Camera 
            {
                ImGui::InputFloat("X", &Camera_Position.x, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Y", &Camera_Position.y, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Zoom", &Z, .1f, 1.0f, "%.3f");
            }


            // Rotation Slider for Camera
            {
                Application::getCamera().Translate(Camera_Position);
                Application::getCamera().s_Zoom(Z);
            }

            // Set the Rotation of the Default Framebuffer
            {


                ImGui::InputFloat("Rotation", &Rot, 1.0f, 1.0f, "%.3f");
                float R = (float)(DEGREES(Application::getCamera().g_Rotation()));
                Application::getCamera().Rotate(Rot - R);// (float)(RADIANS(Rot)));
            }


            // Clickable Position Slider for the Camera 
            {
                ImGui::InputFloat("Width", &W, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Height", &H, 10.0f, 1.0f, "%.3f");
                Application::getCamera().Resize({ W,H });
            }

            // Camera for Test 3D Renderer
            {
                ImGui::InputFloat("3D Px", &Camera_3D->Target_Position.x, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("3D Py", &Camera_3D->Target_Position.y, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("3D Pz", &Camera_3D->Target_Position.z, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Rotation X", &Camera_3D->Rotation.x, .1f, 1.0f, "%.3f");
                ImGui::InputFloat("Rotation Y", &Camera_3D->Rotation.y, .1f, 1.0f, "%.3f");
                ImGui::InputFloat("Rotation Z", &Camera_3D->Rotation.z, .1f, 1.0f, "%.3f");
            }
        }
        ImGui::End();


        ImGui::Begin("Textures");
        {// Displays ALL the created Textures

            float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
            float WinSize = 200;
            ImGuiStyle& style = ImGui::GetStyle();
            uint32_t n = 0;
            for (auto& T : Texture::Texture_Resources)
            {
                // Displays the FrameBuffer  
                ImGui::Image((ImTextureID*)((size_t)T->g_Handle()), ImVec2(WinSize, WinSize));
                float last_button_x2 = ImGui::GetItemRectMax().x;
                float next_button_x2 = last_button_x2 + style.ItemSpacing.x + WinSize; // Expected position if next button was on same line
                if (n + 1 < Texture::Texture_Resources.size() && next_button_x2 < window_visible_x2)
                {
                    ImGui::SameLine();
                }
                ++n;

            }

            ImGui::ShowDemoWindow();
            ImGui::SetWindowPos(ImVec2(0, 0), true); // 768 - ImGui::GetWindowHeight() - 40MainWindowSize.x * 0.15f
            ImGui::SetWindowSize
            (
                {
                    ImGui::GetWindowWidth(),
                    Application::getWindow().Height() - (Application::getWindow().Height()  * 0.2f)
                }
            );
        }
        ImGui::End();




        // Color Picker Widget for the Background Color 
        {
            Vec4 c = Application::getWindow().g_ClearColor();
            float Col[3] = { c.x,c.y,c.z };
            ImGui::ColorPicker3("Background Color", Col, 0);
            glClearColor(Col[0], Col[1], Col[2], 1);
            Application::getWindow().s_ClearColor({ Col[0], Col[1], Col[2], 1 });
        }

    }
    void OnUpdate() override
    {}
};


