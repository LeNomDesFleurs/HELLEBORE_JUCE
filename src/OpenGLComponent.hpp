#include <JuceHeader.h>


struct OpenGLComponent: juce::Component, OpenGLRenderer {
  //   BackgroundComponent(juce::AudioProcessorValueTreeState& apvts);
  OpenGLComponent();
  ~OpenGLComponent();

  void paint(juce::Graphics& g) override;
  //   void resized() override;
  void newOpenGLContextCreated() override;
void renderOpenGL() override;
void openGLContextClosing() override;
private:
  OpenGLContext openGLContext;

    struct Vertex
    {
        float position[2];
        float colour[4];
    };
    
    String vertexShader;
    String fragmentShader;
    std::vector<Vertex> vertexBuffer;
    std::vector<unsigned int> indexBuffer;
    GLuint vbo; // Vertex buffer object.
    GLuint ibo; // Index buffer object.
        std::unique_ptr<OpenGLShaderProgram> shaderProgram;

};