#pragma once

#include <concepts>
#include <type_traits>
#include "../gl/gl.hpp"
#include "../gl/framebuffer.hpp"
#include "../gl/texture.hpp"
#include "stage_context.hpp"

namespace squint::fx {

  /*
   * Intermediate stages must have the following signature:
   *
   * void intermediateStage(const StageContext& context);
   */
  template<typename Func>
  concept IntermediateStage = requires (Func f, const StageContext& context) {
    { f(context) } -> std::same_as<void>;
  };


  /*
   * Terminator stages terminate the current pipeline by returning *something*:
   * 
   * T terminatorStage(const StageContext& context);
   */
  template<typename Func>
  concept TerminatorStage = std::is_invocable_v<Func, const StageContext&> &&
                            !std::is_same_v<std::invoke_result_t<Func, const StageContext&>, void>;

  class Pipeline final {
  public:
    Pipeline(int width, int height);

    template<IntermediateStage Stage>
    Pipeline& operator|(Stage&& stage) {
      stage({ 
        width_, height_,
        *frontTexture_,
        *backTexture_,
        *frontBuffer_,
        *backBuffer_
      });
      prepareNextStage();
      return *this;
    }

    template<TerminatorStage Stage>
    auto operator|(Stage&& stage) {
      auto value = stage({
        width_, height_,
        *frontTexture_,
        *backTexture_,
        *frontBuffer_,
        *backBuffer_
      });
      prepareNextStage();
      return value;
    }

  private:
    int width_, height_;
    gl::Framebuffer frontBuffer_, backBuffer_;
    gl::Texture frontTexture_, backTexture_;

    void prepareNextStage();
  };
  }
