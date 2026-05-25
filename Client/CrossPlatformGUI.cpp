#include "CrossPlatformGUI.h"

bool CrossPlatformGUI::performGUIPass()
{
    //ImGui::NewFrame();

    bool open = true;

    //-----------------------------------------
    // estilo
    //-----------------------------------------

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.FrameRounding = 8.0f;
    style.GrabRounding = 8.0f;

    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;

    style.WindowPadding = ImVec2(20,20);
    style.FramePadding = ImVec2(12,8);
    style.ItemSpacing = ImVec2(10,12);

    //-----------------------------------------
    // usar TODO el viewport
    //-----------------------------------------

	ImVec2 size = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowPos(
		ImVec2(0,0),
		ImGuiCond_Always
	);

	ImGui::SetNextWindowSize(
		size,
		ImGuiCond_Always
	);

    ImGui::Begin(
        "MainWindow",
        &open,

        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings
    );

    //-----------------------------------------
    // Header
    //-----------------------------------------

    ImGui::SetWindowFontScale(1.4f);

    ImGui::Text(
        "Sony WH Headphones"
    );

    ImGui::SetWindowFontScale(1.0f);

    ImGui::Separator();

    if(this->_bt.isConnected())
    {
        ImGui::Text("Status:");
        ImGui::SameLine();

        ImGui::TextColored(
            ImVec4(
                0.2f,
                0.9f,
                0.3f,
                1.0f
            ),
            "CONNECTED"
        );

        ImGui::Text(
            "Device: %s",
            this->_connectedDevice.name.c_str()
        );
    }
    else
    {
        ImGui::Text("Status:");
        ImGui::SameLine();

        ImGui::TextColored(
            ImVec4(
                0.9f,
                0.2f,
                0.2f,
                1.0f
            ),
            "DISCONNECTED"
        );
    }

    ImGui::Spacing();

    ImGui::Separator();

    ImGui::Text("Connection");

    ImGui::Separator();

    this->_drawErrors();

    this->_drawDeviceDiscovery();

    //-----------------------------------------

    if(this->_bt.isConnected())
    {
        ImGui::Spacing();

        ImGui::Separator();

        ImGui::Text(
            "Noise Control"
        );

        ImGui::Separator();

        this->_drawASMControls();

        ImGui::Spacing();

        ImGui::Separator();

        ImGui::Text(
            "Spatial Audio"
        );

        ImGui::Separator();

        this->_drawSurroundControls();

        ImGui::Spacing();

        this->_setHeadphoneSettings();
    }

    ImGui::Spacing();

    ImGui::Separator();

    ImGui::TextDisabled(
        "Unofficial Sony client"
    );

    ImGui::End();

    //ImGui::Render();

    return open;
}


void CrossPlatformGUI::_drawErrors()
{
    if (this->_mq.begin() != this->_mq.end())
    {
        ImGui::TextColored(
            ImVec4(1.0f,0.3f,0.3f,1.0f),
            "Errors:"
        );

        ImGui::Spacing();

        for (auto&& message : this->_mq)
        {
            ImGui::BulletText(
                "%s",
                message.message.c_str()
            );
        }

        ImGui::Spacing();
    }
}


void CrossPlatformGUI::_drawDeviceDiscovery()
{
    static std::vector<BluetoothDevice>
        connectedDevices;

    static int selectedDevice=-1;

    if (this->_bt.isConnected())
    {
        ImGui::Text(
            "Connected to:"
        );

        ImGui::SameLine();

        ImGui::TextColored(
            ImVec4(0.2f,0.8f,0.3f,1.0f),
            "%s",
            this->_connectedDevice.name.c_str()
        );

        if(ImGui::Button(
            "Disconnect",
            ImVec2(150,35)
        ))
        {
            selectedDevice=-1;

            this->_bt.disconnect();
        }

        return;
    }

    ImGui::Text(
        "Select a device:"
    );

    ImGui::Spacing();

    int temp=0;

    for(const auto& device:
        connectedDevices)
    {
        ImGui::RadioButton(
            device.name.c_str(),
            &selectedDevice,
            temp++
        );
    }

    ImGui::Spacing();

    if(this->_connectFuture.valid())
    {
        if(this->_connectFuture.ready())
        {
            try
            {
                this->_connectFuture.get();
            }
            catch(
                const RecoverableException&
                exc
            )
            {
                if(exc.shouldDisconnect)
                {
                    this->_bt.disconnect();
                }

                this->_mq.addMessage(
                    exc.what()
                );
            }
        }
        else
        {
            ImGui::Text(
                "Connecting..."
            );
        }
    }
    else
    {
        if(ImGui::Button(
            "Connect",
            ImVec2(150,35)
        ))
        {
            if(selectedDevice!=-1)
            {
                this->_connectedDevice=
                    connectedDevices[
                        selectedDevice
                    ];

                this->_connectFuture
                .setFromAsync(
                [this]()
                {
                    this->_bt.connect(
                    this->_connectedDevice.mac
                    );
                });
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button(
        "Refresh",
        ImVec2(150,35)
    ))
    {
        selectedDevice=-1;

        this->_connectedDevicesFuture
        .setFromAsync(
        [this]()
        {
            return this->_bt
            .getConnectedDevices();
        });
    }

    if(this->_connectedDevicesFuture.valid())
    {
        if(this->_connectedDevicesFuture.ready())
        {
            try
            {
                connectedDevices=
                this->_connectedDevicesFuture
                .get();
            }
            catch(
                const RecoverableException&
                exc
            )
            {
                this->_mq.addMessage(
                    exc.what()
                );
            }
        }
    }
}


void CrossPlatformGUI::_drawASMControls()
{
    static bool ambientSoundControl=true;
    static bool focusOnVoice=false;
    static int asmLevel=0;

    ImGui::Checkbox(
        "Enable Ambient Sound",
        &ambientSoundControl
    );

    if(ambientSoundControl)
    {
        ImGui::Indent();

        ImGui::Text(
            "Environment Level"
        );

        ImGui::SliderInt(
            "##ambient",
            &asmLevel,
            0,
            19
        );

        if(
        this->_headphones
        .isFocusOnVoiceAvailable()
        )
        {
            ImGui::Checkbox(
                "Focus on Voice",
                &focusOnVoice
            );
        }
        else
        {
            ImGui::TextDisabled(
                "Focus on Voice unavailable"
            );
        }

        ImGui::Unindent();
    }

    this->_headphones
        .setAmbientSoundControl(
            ambientSoundControl
        );

    this->_headphones
        .setAsmLevel(
            asmLevel
        );

    this->_headphones
        .setFocusOnVoice(
            focusOnVoice
        );
}


void CrossPlatformGUI::_drawSurroundControls()
{
    static int soundPosition=0;
    static int vptType=0;

    ImGui::TextDisabled(
        "Only one mode can be active"
    );

    ImGui::Spacing();

    if(
    ImGui::Combo(
        "Position",
        &soundPosition,
        "Off\0Front Left\0Front Right\0Front\0Rear Left\0Rear Right\0\0"
    ))
    {
        vptType=0;
    }

    if(
    ImGui::Combo(
        "Environment",
        &vptType,
        "Off\0Outdoor Festival\0Arena\0Concert Hall\0Club\0\0"
    ))
    {
        soundPosition=0;
    }

    this->_headphones
    .setSurroundPosition(
        SOUND_POSITION_PRESET_ARRAY[
            soundPosition
        ]
    );

    this->_headphones
    .setVptType(
        vptType
    );
}


void CrossPlatformGUI::_setHeadphoneSettings()
{
    static int commandLinger=0;

    if(this->_sendCommandFuture.ready())
    {
        commandLinger=0;

        try
        {
            this->_sendCommandFuture.get();
        }
        catch(
            const RecoverableException&
            exc
        )
        {
            std::string excString;

            if(exc.shouldDisconnect)
            {
                this->_bt.disconnect();

                excString=
                "Disconnected: ";
            }

            this->_mq.addMessage(
                excString+
                exc.what()
            );
        }
    }
    else if(
        this->_sendCommandFuture
        .valid()
    )
    {
        if(
        commandLinger++
        >(FPS/10)
        )
        {
            ImGui::Text(
                "Sending..."
            );
        }
    }
    else if(
        this->_headphones
        .isChanged()
    )
    {
        this->_sendCommandFuture
        .setFromAsync(
        [=,this]()
        {
            return this
            ->_headphones
            .setChanges();
        });
    }
}


CrossPlatformGUI::CrossPlatformGUI(
BluetoothWrapper bt
)
:
_bt(std::move(bt)),
_headphones(_bt)
{
    ImGui::StyleColorsDark();

    ImGuiIO& io=
        ImGui::GetIO();

    this->_mq=
        TimedMessageQueue(
            GUI_MAX_MESSAGES
        );

    this
    ->_connectedDevicesFuture
    .setFromAsync(
    [this]()
    {
        return this->_bt
        .getConnectedDevices();
    });

    io.IniFilename=nullptr;

    io.WantSaveIniSettings=
        false;

    char* fileData=
    new char[
    sizeof(
    CascadiaCodeTTF
    )];

    memcpy(
        fileData,
        CascadiaCodeTTF,
        sizeof(
        CascadiaCodeTTF
        )
    );

    ImFont* font=
    io.Fonts
    ->AddFontFromMemoryTTF(
        reinterpret_cast<void*>(
            fileData
        ),
        sizeof(
            CascadiaCodeTTF
        ),
        FONT_SIZE
    );

    IM_ASSERT(font!=NULL);
}