class IRenderer
{
public:
  IRenderer(/* args */) = default;
  ~IRenderer() = default;

  virtual void run(float deltaTime) = 0;

  virtual bool init() = 0;
  virtual bool isInitialized() = 0;

  virtual void quit() = 0;

  virtual bool shouldClose() = 0;
};
