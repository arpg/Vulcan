#pragma once

#include <vulcan/tracker.h>
#include <vulcan/image.h>
#include <vulcan/light.h>

namespace vulcan
{

class LightTracker : public Tracker
{
  public:

    LightTracker();

    virtual ~LightTracker();

    const Light& GetLight() const;

    void SetLight(const Light& light);

    void ComputeResiduals(const Frame& frame, Buffer<float>& residuals);

    void ComputeJacobian(const Frame& frame, Buffer<Vector6f>& jacobian);

  protected:

    void BeginSolve(const Frame& frame) override;

    void ComputeFrameMask(const Frame& frame);

    int GetResidualCount(const Frame &frame) const override;

    void ApplyUpdate(Frame& frame, Eigen::VectorXf& x) const override;

    void ComputeSystem(const Frame& frame) override;

    void ComputeKeyframeIntensities();

    void ComputeFrameIntensities(const Frame& frame);

    void ComputeFrameGradients(const Frame& frame);

    void WriteDataFiles(const Frame& frame);

    void WriteImage(const Frame& frame);

    // void TraceImage(const Frame& frame, Image& data);
    void TraceImage(const Frame& frame, ColorImage& data);

  protected:

    Light light_;

    Image frame_mask_;

    Image keyframe_intensities_;

    Image frame_intensities_;

    Image frame_gradient_x_;

    Image frame_gradient_y_;

    float depth_threshold_;

    int frame_;

  public:

    bool write_;
};

} // namespace vulcan