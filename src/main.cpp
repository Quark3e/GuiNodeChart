
/**
 * @file main.cpp
 * @author Erkhme Byambadorj (b.erkhme@gmail.com)
 * @brief ImGui based nodeChart
 * @version 0.1
 * @date 2024-08-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "globals_includes.hpp"
#include "globals_variables.hpp"


int main(int argc, char** argv) {

    if(argc>1) {
        std::cout << "program entered arguments:" << std::endl;
        for(int i=0; i<argc; i++) {
            std::cout << "  \""<<argv[i]<<"\""<<std::endl;
            if(strcmp(argv[i], "-T")==0 || strcmp(argv[i], "--TERMINAL")==0) {
                __ON_TERMINAL = true;
            }
        }
    }
    std::cout << std::endl;
    

    std::cout << "Program configuration: ";
    if(__ON_TERMINAL) std::cout << "Terminal";
    else std::cout << "Default";
    std::cout << std::endl;

    // programCWD = getFileCWD(true);
    // std::cout << programCWD << std::endl;
    std::string default_proj_file = __dir_saveFiles + "default_chart.json";
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* display = al_create_display(dim__main.x, dim__main.y);
    al_set_window_title(display, "al window title");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); //(void)io;
    ImGuiStyle& style = ImGui::GetStyle();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = NULL; //(__dir_programRoot+"imgui.ini").c_str();

    ImGui::StyleColorsDark();
    ImGui_ImplAllegro5_Init(display);

    ImVec4 clear_color = ImVec4(0.20f, 0.20f, 0.2f, 0.88f);

    static ImGuiWindowFlags window0_flags   = 0;
    static ImGuiWindowFlags window1_flags   = 0;
    static ImGuiChildFlags  child0_flags    = 0;

    window0_flags |= ImGuiWindowFlags_NoMove;
    window0_flags |= ImGuiWindowFlags_NoResize;
    window0_flags |= ImGuiWindowFlags_NoCollapse;
    // window0_flags |= ImGuiWindowFlags_MenuBar;
    window0_flags |= ImGuiWindowFlags_NoTitleBar;
    window0_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    
    window1_flags |= ImGuiWindowFlags_NoMove;
    window1_flags |= ImGuiWindowFlags_NoResize;
    window1_flags |= ImGuiWindowFlags_NoCollapse;
    window1_flags |= ImGuiWindowFlags_MenuBar;
    window1_flags |= ImGuiWindowFlags_NoTitleBar;
    
    // gNC::guiNodeChart proj0;
    // proj0.thisPtr = &proj0;

    
    projects.add("project 0", nc_proj{"", false, gNC::guiNodeChart()});
    projects[0].chart.thisPtr = &(projects[0].chart);

    // projects[0].chart.project_name = projects.key(0);
    projects.rename("project 0", ptrToStr<gNC::guiNodeChart*>(projects[0].chart.thisPtr));

    projects[0].chart.loadFile(default_proj_file, false);
    projects[-1].chart.screen_dim[0] = dim__main.x;
    projects[-1].chart.screen_dim[1] = dim__main.y;
    projects[0].fileLinked = true;
    projects[0].filename = default_proj_file;

    projects[-1].chart.TimeLine.set_channel_limit(0);
    projects[-1].chart.TimeLine.add_timeObject(
        &projects[-1].chart[0], 0, 50, 1
    );
    projects[-1].chart.TimeLine.add_timeObject(
        &projects[-1].chart[1], 10, 45, 2
    );
    projects[-1].chart.TimeLine.add_timeObject(
        &projects[-1].chart[2], 37, 169, 3
    );
    projects[-1].chart.TimeLine.add_timeObject(
        &projects[-1].chart[3], 75, 210, 4
    );
    projects[-1].chart.TimeLine.add_timeObject(
        &projects[-1].chart[4], 81, 240, 1
    );


    int _selected = 0, _selected_prev = 0;
    int tabAddNew = 0;

    static int cnt = 0;

    bool setting_autosave = true;
    int setting_autosave_iterCount = 0;
    int setting_autosave_iterWait = 60; //how many frames/iterations to wait before saving




    // _SETTINGS[0][0] = true;
    while(_SETTINGS[0][0]) {
        __PROGRAM_FRAMES++;
        static std::vector<std::vector<int>>* pressed_keys;
        pressed_keys = &guiKeys.pressed;
        guiKeys.update();
        keyBinds.update(guiKeys.pressed[guiKeys.pressed.size()-1]);
        // update_keys();
        ALLEGRO_EVENT al_event;
        while (al_get_next_event(queue, &al_event)) {
            ImGui_ImplAllegro5_ProcessEvent(&al_event);
            if(al_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                // running_main = false;
                _SETTINGS[0][0] = false;
                __GLOBAL_FLAGS__WIN_CLOSING = 1;
            }
            if(al_event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
                ImGui_ImplAllegro5_InvalidateDeviceObjects();
                al_acknowledge_resize(display);
                ImGui_ImplAllegro5_CreateDeviceObjects();
                projects[_selected].chart.screen_dim[0] = al_get_display_width(display);
                projects[_selected].chart.screen_dim[1] = al_get_display_height(display);
                dim__main.x = projects[_selected].chart.screen_dim[0];
                dim__main.y = projects[_selected].chart.screen_dim[1];
                __GLOBAL_FLAGS__WIN_RESIZED = 2;
                if(__ON_TERMINAL) {
                    std::cout<<"["<<GET_CURRENT_TIME()<<"]: ";
                    std::cout << "window_resized: ["<<dim__main.x<< ", "<<dim__main.y<<"]"<< std::endl;
                }
            }
        }
        
        ImGui_ImplAllegro5_NewFrame();
        ImGui::NewFrame();
        //--------------------
        ImGui::SetNextWindowSizeConstraints(dim__main, dim__main);
        style.WindowRounding = 0;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 0));
        ImGui::Begin("main__content", NULL, window0_flags);
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(dim__main);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();



        if(_selected!=-1) {
            project_draw_list = ImGui::GetWindowDrawList();

            static ImVec2 relativePos(0, 0);
            relativePos = ImVec2(io.MousePos.x/dim__main[0], io.MousePos.y/dim__main[1]);
            if(io.MouseWheel!=0 && !gNC::_DRAW_SCALAR_SCOLL_LOCK) {
                ImVec2 _prevPos(
                    (1.0/gNC::_DRAW_SCALAR.x)*(io.MousePos.x) - projects[_selected].chart.screen_pos[0],
                    (1.0/gNC::_DRAW_SCALAR.y)*(io.MousePos.y) - projects[_selected].chart.screen_pos[1]
                );
                gNC::_DRAW_SCALAR.x += drawScalar_mouseWheelStep * io.MouseWheel;
                gNC::_DRAW_SCALAR.y += drawScalar_mouseWheelStep * io.MouseWheel;
                projects[_selected].chart.setScreen_pos(
                    (dim__main[0]*relativePos.x*(1.0/gNC::_DRAW_SCALAR.x)) - _prevPos.x,
                    (dim__main[1]*relativePos.y*(1.0/gNC::_DRAW_SCALAR.y)) - _prevPos.y
                );
            }
            else {
                gNC::_DRAW_SCALAR_SCOLL_LOCK = false;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 0));
            assert(ImGui::BeginChild("tab0: contents", ImVec2(0, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_NoMove));
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            
            if(ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)) {
                if(!lockMove_screen && isKeyPressed(655, &((*pressed_keys)[pressed_keys->size()-1]))) {
                    projects[_selected].chart.setScreen_pos(io.MouseDelta.x*(1.0/gNC::_DRAW_SCALAR.x), io.MouseDelta.y*(1.0/gNC::_DRAW_SCALAR.y), 1); //??
                    // std::cout << "new screen pos: " << Useful_GuiNodeChart::formatContainer1(projects[_selected].chart.screen_pos, 2, 0, 0) << std::endl;
                    mouseDrag_left = true;
                }
                else {
                    mouseDrag_left = false;
                }
            }

            ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
            ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
            if(canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
            if(canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
            ImVec2 canvas_p1 = ImVec2(canvas_p0.x+canvas_sz.x, canvas_p0.y+canvas_sz.y);
            canvas_p1 = ImVec2_divide(canvas_p1, gNC::_DRAW_SCALAR);

            static const int GRID_STEP = 64;
            // Vec2i scal_GRID_STEP = ImVec2_multiply(gNC::_DRAW_SCALAR, ImVec2(GRID_STEP, GRID_STEP));
            if(_SETTINGS.get("View").get("Draw Grid")) {
                draw_list->PushClipRect(ImVec2(0, 20), ImVec2(dim__main[0], dim__main[1]), true);
                for(float x=0; x<dim__main[0]/gNC::_DRAW_SCALAR.x; x+=GRID_STEP)
                    draw_list->AddLine(
                        ImVec2_multiply(ImVec2(x+(projects[_selected].chart.screen_pos[0] % GRID_STEP), canvas_p0.y), gNC::_DRAW_SCALAR),
                        ImVec2_multiply(ImVec2(x+(projects[_selected].chart.screen_pos[0] % GRID_STEP), canvas_p1.y), gNC::_DRAW_SCALAR),
                        IM_COL32(200, 200, 200, 40)
                    );

                for(float y=0; y<dim__main[1]/gNC::_DRAW_SCALAR.y; y+=GRID_STEP)
                    draw_list->AddLine(
                        ImVec2_multiply(ImVec2(canvas_p0.x, y+(projects[_selected].chart.screen_pos[1] % GRID_STEP)), gNC::_DRAW_SCALAR),
                        ImVec2_multiply(ImVec2(canvas_p1.x, y+(projects[_selected].chart.screen_pos[1] % GRID_STEP)), gNC::_DRAW_SCALAR),
                        IM_COL32(200, 200, 200, 40)
                    );
                draw_list->PopClipRect();
            }


            // if(cnt==0) {
            //     projects[_selected].chart.NODE_create(100, 100, "node0", "desc0", "body0");
            //     projects[_selected].chart.NODE_create(400, 200, "node1", "desc1", "body1");
            //     projects[_selected].chart.NODE_create(600, 500, "node2", "desc2", "body2");
            //     projects[_selected].chart.NODE_create(900, 300, "node3", "desc3", "body3");
            //     projects[_selected].chart.NODE_create(100, 300, "node4", "desc4", "body4");
            //     projects[_selected].chart.NODE_create(100, 600, "node5", "desc5", "body5");
            //     projects[_selected].chart[1].bodyText = std::string("")+
            //         "What is Lorem Ipsum? "+
            //         "Lorem Ipsum is simply dummy text of the printing and typesetting industry. "+
            //         "Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, \n"+
            //         "when an unknown printer took a galley of type and scrambled it to make a type specimen book. \n"+
            //         "It has survived not only five centuries, but also the leap into electronic typesetting, \n"+
            //         "remaining essentially unchanged. \n"+
            //         "It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, \n"+
            //         "and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.\n";
            //     projects[_selected].chart.LINK_create(0, 1, 1, 0, "link: 0->1", "a link that goes out from node0 in to node 1");
            //     // projects[_selected].chart.LINK_create(&proj0[0], &proj0[2], 1, 0, "link0");
            //     projects[_selected].chart.LINK_create(4, 1, 1, 0, "link: 4->1", "a link that goes out from node0 in to node 1");
            //     projects[_selected].chart.LINK_create(5, 1, 1, 0, "link: 5->2", "a link that goes out from node0 in to node 1");
            //     projects[_selected].chart.LINK_create(1, 2, 1, 2, "link: 1->2", "a link that goes out from node1 added to node 2");
            //     projects[_selected].chart.LINK_create(1, 3, 1, 0, "link: 1->3", "a link that goes out from node1 in to node3");
            //     // projects[_selected].chart.NODE_delete(&proj0[1], true);
            // }
            style.WindowRounding = 15.0f;
            projects[_selected].chart.draw();


            // ImGui::SetCursorPos(ImVec2(10, dim__main[1] - 25 - ImGui::GetTextLineHeightWithSpacing()*4));
            // ImGui::Text("Rel pos:    x:%2.1f y:%2.1f", relativePos.x*100.0, relativePos.y*100.0);
            // ImGui::SetCursorPos(ImVec2(10, dim__main[1] - 25 - ImGui::GetTextLineHeightWithSpacing()*3));
            // ImGui::Text("Screen pos:x:%3.1d y:%3.1d", projects[_selected].chart.screen_pos[0], projects[_selected].chart.screen_pos[1]);
            ImGui::SetCursorPos(ImVec2(10, dim__main[1] - 25 - ImGui::GetTextLineHeightWithSpacing()*2));
            ImGui::Text("Chart pos: x:%3.1f y:%3.1f", (1.0/gNC::_DRAW_SCALAR.x)*(io.MousePos.x) - projects[_selected].chart.screen_pos[0], (1.0/gNC::_DRAW_SCALAR.y)*(io.MousePos.y) - projects[_selected].chart.screen_pos[1]);

            ImGui::EndChild();

        }
        
        ImGui::SetCursorPos(ImVec2(10, dim__main[1] - 25 - ImGui::GetTextLineHeightWithSpacing()*1));
        ImGui::Text("Mouse pos: x:%3.1f y:%3.1f", io.MousePos.x, io.MousePos.y);
        ImGui::SetCursorPosX(10);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f/io.Framerate, io.Framerate);
        ImGui::End();
        cnt++;

        // ----------

        style.WindowRounding = 0;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 0));
        ImGui::Begin("main__overlay", NULL, window1_flags);
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(dim__main[0], 45));


        if(ImGui::BeginMenuBar()) {
            if(ImGui::BeginMenu("File")) {
                if(ImGui::MenuItem("New project")) {
                    projects.add("_temp", nc_proj{"", false, gNC::guiNodeChart()});
                    projects[-1].chart.thisPtr = &(projects[-1].chart);
                    projects.rename("_temp", ptrToStr<gNC::guiNodeChart*>(&(projects[-1].chart)));
                    _selected = projects.size()-1;
                }
                ImGui::Separator();
                if(ImGui::MenuItem("Open file")) {
                    if(gNC::_mode__fileExplorer==0) {
                        gNC::_mode__fileExplorer = 1;
                        gNC::_valid__extensions = std::vector<std::string>{"json"};
                    }
                    
                    // nodePtr_menu__node_details  = nullptr;
                    // linkPtr_menu__link_details  = nullptr;
                    // nodePtr_menu__rightClick    = nullptr;
                    // linkPtr_menu__rightClick    = nullptr;
                }
                if(ImGui::MenuItem("Load file")) {
                    if(gNC::_mode__fileExplorer==0) {
                        gNC::_mode__fileExplorer = 3;
                        gNC::_valid__extensions = std::vector<std::string>{"json"};
                    }
                }
                ImGui::Separator();
                if(ImGui::MenuItem("Save")) {

                    if(projects[_selected].fileLinked) {
                        if(projects[_selected].chart.modified) {
                            projects[_selected].chart.saveToFile(projects[_selected].filename, true);
                            projects[_selected].chart.modified = false;
                        }
                    }
                    else {
                        if(gNC::_mode__fileExplorer==0) {
                            gNC::_mode__fileExplorer = 2;
                            gNC::_valid__extensions = std::vector<std::string>{"json"};
                        }
                    }
                    
                }
                if(ImGui::MenuItem("Save as")) {
                    /**
                     * @brief Normally this would open a dedicated "file dialog" window but now for testing sake it'll just
                     * directly call the `gNC::guiNodeChart::saveToFile` member function
                     * 
                     */
                    // projects[_selected].chart.saveToFile(programCWD+"saveFiles/_TEST_" + "" + "project 0.json", true);
                    if(gNC::_mode__fileExplorer==0) {
                        gNC::_mode__fileExplorer = 4;
                        gNC::_valid__extensions = std::vector<std::string>{"json"};
                    }
                }
                ImGui::Separator();
                // ImGui::Checkbox("AutoSave", &setting_autosave);
                
                auto& _ref = _SETTINGS.get("File");
                for(size_t i=0; i<_ref.size(); i++) {
                    ImGui::Checkbox(_ref.getKey(i).c_str(), _ref.getPtr_idx(i));
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Project")) {
                if(ImGui::MenuItem("Clear")) {
                    projects[_selected].chart = gNC::guiNodeChart();
                    projects[_selected].fileLinked = false;
                    projects[_selected].filename = "";
                    projects[_selected].chart.thisPtr = &(projects[_selected].chart);
                    if(ptrToStr<gNC::guiNodeChart*>(&(projects[_selected].chart)) != projects.key(_selected)) {
                        projects[_selected].chart.thisPtr = &(projects[_selected].chart);
                        projects.rename(projects.key(_selected), ptrToStr<gNC::guiNodeChart*>(&(projects[_selected].chart)));
                    }
                    gNC::reset_Ptr_menus();
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Program")) {
                if(ImGui::MenuItem("Close")){ _SETTINGS.get("Program").get("Running Main") = false; }
                // if(ImGui::MenuItem(""))
                dim__main__drawScalar = gNC::_DRAW_SCALAR;
                float _inputScalar = dim__main__drawScalar.x;
                ImGui::SliderFloat("Scalar", &_inputScalar, 0.1f, 2);
                dim__main__drawScalar.x = _inputScalar;
                dim__main__drawScalar.y = _inputScalar;
                gNC::_DRAW_SCALAR = dim__main__drawScalar;
                
                // std::cout << Useful_GuiNodeChart::formatContainer1(gNC::_DRAW_SCALAR, 2, 5, 2) << std::endl;
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View")) {
                auto& _ref = _SETTINGS.get("View");
                for(size_t i=0; i< _SETTINGS.get("View").size(); i++) {
                    ImGui::Checkbox(_SETTINGS.get("View").getKey(i).c_str(), _SETTINGS.get("View").getPtr_idx(i));
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Timeline")) {
                auto& _ref = _SETTINGS.get("Timeline");
                for(size_t i=0; i<_ref.size(); i++) {
                    ImGui::Checkbox(_ref.getKey(i).c_str(), _ref.getPtr_idx(i));
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if(ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_AutoSelectNewTabs)) {
            for(size_t i=0; i<projects.size(); i++) {
                bool open = true;
                if(ImGui::BeginTabItem(("project "+std::to_string(i)).c_str(), &open, (projects[i].chart.modified? ImGuiTabItemFlags_UnsavedDocument : ImGuiTabItemFlags_None))) {
                    _selected_prev = _selected;
                    _selected = i;
                    ImGui::EndTabItem();
                }
                // if(i==0) std::cout << "open: "<<std::boolalpha << open << std::endl;
                if(!open) {
                    projects.eraseIdx(i);
                    if(_selected>=i) _selected--;
                    if(_selected_prev>=i) _selected_prev--;
                }
            }
            if(ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing)) {
                projects.add("_temp", nc_proj{"", false, gNC::guiNodeChart()});
                projects[-1].chart.thisPtr = &(projects[-1].chart);
                projects.rename("_temp", ptrToStr<gNC::guiNodeChart*>(&(projects[-1].chart)));
                _selected = projects.size()-1;
            }
            ImGui::EndTabBar();
        }
        if(_selected != _selected_prev) {
            gNC::nodePtr_menu__node_details = nullptr;
            gNC::linkPtr_menu__link_details = nullptr;
            gNC::nodePtr_menu__rightClick = nullptr;
            gNC::linkPtr_menu__rightClick = nullptr;
        }
        if(_selected!=-1) gNC::_menu__timeline(&projects[_selected].chart);
        gNC::_menu__fileExplorer();
        if(!gNC::_mode__fileExplorer && gNC::_file__fileExplorer.length()!=0) {
            switch (gNC::_mode__fileExplorer_prev) {
                case 1: { //open
                    projects.add("_temp", nc_proj{gNC::_file__fileExplorer, true, gNC::guiNodeChart()});
                    projects[-1].chart.thisPtr = &(projects[-1].chart);
                    projects.rename("_temp", ptrToStr<gNC::guiNodeChart*>(&(projects[-1].chart)));
                    // projects[-1.chart].loadFile(programCWD+"saveFiles/_TEST_"+""+ "project 0.json");

                    projects[-1].chart.loadFile(gNC::_file__fileExplorer);
                    projects[-1].chart.screen_dim[0] = dim__main.x;
                    projects[-1].chart.screen_dim[1] = dim__main.y;
                    _selected = projects.size()-1;

                    projects[-1].chart.modified = false;
                    break;
                }
                case 2: { //save
                    projects[_selected].chart.saveToFile(gNC::_file__fileExplorer, true);
                    projects[_selected].fileLinked = true;
                    projects[_selected].chart.modified = false;
                    break;
                }
                case 3: { //load
                    projects[_selected].chart.loadFile(gNC::_file__fileExplorer);
                    projects[_selected].chart.modified = false;
                    break;
                }
                case 4: { //save as
                    projects[_selected].chart.saveToFile(gNC::_file__fileExplorer, true);
                    projects[_selected].fileLinked = true;
                    projects[_selected].chart.modified = false;
                    break;
                }
                default: std::runtime_error("Oopsie daisy you fucked up"); break;
            }
        }

        if(setting_autosave) {
            if(setting_autosave_iterCount>=setting_autosave_iterWait) {
                for(int i=0; i<projects.size(); i++) {
                    if(projects[i].fileLinked && projects[i].chart.modified) {
                        projects[i].chart.saveToFile(projects[i].filename, true);
                        projects[i].chart.modified = false;
                    }
                }
                setting_autosave_iterCount = 0;
            }
            else {
                setting_autosave_iterCount++;
            }
        }


        ImGui::End();
        //--------------------
        ImGui::Render();
        al_clear_to_color(al_map_rgba_f(clear_color.x*clear_color.w, clear_color.y*clear_color.w, clear_color.z*clear_color.w, clear_color.w));
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        al_flip_display();
        #if _DEBUG
        // std::cout << "\n";
        // std::cout.flush();
        #endif


        if(__GLOBAL_FLAGS__WIN_RESIZED>0) {
            __GLOBAL_FLAGS__WIN_RESIZED = (__GLOBAL_FLAGS__WIN_RESIZED==1? 0 : __GLOBAL_FLAGS__WIN_RESIZED-1);
        }
        // exit(0);
    }
    
    // for(size_t i=0; i<_SETTINGS.size(); i++) {
    //     std::cout << _SETTINGS[i] << " ";
    // }
    // std::cout << std::endl;


    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}

