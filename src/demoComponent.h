#pragma once


//==============================================================================
class OpenGLDemo2D final : public Component
{
public:
    OpenGLDemo2D()
    {
        setOpaque (true);

        if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine (0);

        openGLContext.attachTo (*getTopLevelComponent());

        // setSize (500, 500);
    }

    ~OpenGLDemo2D() override
    {
        openGLContext.detach();
        shader.reset();
    }

    void paint (Graphics& g) override
    {
        g.fillCheckerBoard (getLocalBounds().toFloat(), 48.0f, 48.0f, Colours::lightgrey, Colours::white);

        if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
        {
            shader.reset();

            if (fragmentCode.isNotEmpty())
            {
                shader.reset (new OpenGLGraphicsContextCustomShader (fragmentCode));

                auto result = shader->checkCompilation (g.getInternalContext());

                if (result.failed())
                {
                    shader.reset();
                }
            }
        }

        if (shader.get() != nullptr)
        {
            // statusLabel.setText ({}, dontSendNotification);

            shader->fillRect (g.getInternalContext(), getLocalBounds());
        }
    }

    void resized() override
    {
    }


    std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;

    String fragmentCode = "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " float alpha = pixelPos.x / 1000.0;\n"
                "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
                "}\n";

private:
    OpenGLContext openGLContext;


/*
            {
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " float alpha = pixelPos.x / 1000.0;\n"
                "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
                "}\n"
            },

            {
                "Circular Gradient",

                SHADER_2DDEMO_HEADER
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.3, 0.4, 0.4, 1.0);\n"
                "    " JUCE_MEDIUMP " float alpha = distance (pixelPos, vec2 (600.0, 500.0)) / 400.0;\n"
                "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
                "}\n"
            },

            {
                "Circle",

                SHADER_2DDEMO_HEADER
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (0.1, 0.1, 0.9, 1.0);\n"
                "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " float distance = distance (pixelPos, vec2 (600.0, 500.0));\n"
                "\n"
                "    " JUCE_MEDIUMP " float innerRadius = 200.0;\n"
                "    " JUCE_MEDIUMP " float outerRadius = 210.0;\n"
                "\n"
                "    if (distance < innerRadius)\n"
                "        gl_FragColor = colour1;\n"
                "    else if (distance > outerRadius)\n"
                "        gl_FragColor = colour2;\n"
                "    else\n"
                "        gl_FragColor = mix (colour1, colour2, (distance - innerRadius) / (outerRadius - innerRadius));\n"
                "\n"
                "    gl_FragColor *= pixelAlpha;\n"
                "}\n"
            },

            {
                "Solid Colour",

                SHADER_2DDEMO_HEADER
                "void main()\n"
                "{\n"
                "    gl_FragColor = vec4 (1.0, 0.6, 0.1, pixelAlpha);\n"
                "}\n"
            }
        };
*/
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLDemo2D)
};