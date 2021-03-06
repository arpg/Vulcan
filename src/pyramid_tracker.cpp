#include <vulcan/pyramid_tracker.h>
#include <vulcan/frame.h>
#include <vulcan/color_tracker.h>
#include <vulcan/depth_tracker.h>
#include <vulcan/light_tracker.h>

namespace vulcan
{

template<typename Tracker>
PyramidTracker<Tracker>::PyramidTracker() :
  tracker_(std::shared_ptr<Tracker>(new Tracker())),
  half_keyframe_(std::make_shared<Frame>()),
  quarter_keyframe_(std::make_shared<Frame>()),
  iter_(0)
{
}

template<typename Tracker>
PyramidTracker<Tracker>::PyramidTracker(std::shared_ptr<Tracker> tracker) :
  tracker_(tracker),
  half_keyframe_(std::make_shared<Frame>()),
  quarter_keyframe_(std::make_shared<Frame>()),
  iter_(0)
{
}

template<typename Tracker>
PyramidTracker<Tracker>::~PyramidTracker()
{
}

template<typename Tracker>
std::shared_ptr<const Tracker> PyramidTracker<Tracker>::GetTracker() const
{
  return tracker_;
}

template<typename Tracker>
std::shared_ptr<const Frame> PyramidTracker<Tracker>::GetKeyframe() const
{
  return keyframe_;
}

template<typename Tracker>
void PyramidTracker<Tracker>::SetKeyframe(std::shared_ptr<const Frame> keyframe)
{
  keyframe_ = keyframe;
}

template<typename Tracker>
void PyramidTracker<Tracker>::Track(Frame& frame)
{
  VULCAN_DEBUG(keyframe_);

  // // if (++iter_ != 47)
  // // {
  // // std::cout << "Tracker::Iteration: " << iter_ << std::endl;

  Frame half_frame;
  Frame quarter_frame;

  frame.Downsample(half_frame);
  half_frame.Downsample(quarter_frame);

  keyframe_->Downsample(*half_keyframe_);
  half_keyframe_->Downsample(*quarter_keyframe_);

  // tracker_->SetMaxIterations(5);
  // tracker_->SetTranslationEnabled(false);
  // tracker_->SetKeyframe(quarter_keyframe_);
  // tracker_->Track(quarter_frame);

  // tracker_->SetMaxIterations(10);
  // tracker_->SetTranslationEnabled(true);
  // tracker_->SetKeyframe(quarter_keyframe_);
  // tracker_->Track(quarter_frame);

  tracker_->SetMaxIterations(15);
  tracker_->SetTranslationEnabled(true);
  half_frame.depth_to_world_transform = quarter_frame.depth_to_world_transform;
  tracker_->SetKeyframe(half_keyframe_);
  tracker_->Track(half_frame);

  tracker_->SetMaxIterations(20);
  tracker_->SetTranslationEnabled(true);
  frame.depth_to_world_transform = half_frame.depth_to_world_transform;
  tracker_->SetKeyframe(keyframe_);
  tracker_->Track(frame);

  // // }
  // // else
  // // {

  // // std::cout << "Tracker::Investigating..." << std::endl;

  // // Matrix3f R;

  // // R(0, 0) = 0.99999794;
  // // R(1, 0) = -0.00172978;
  // // R(2, 0) = -0.00059492;

  // // R(0, 1) = 0.00173206;
  // // R(1, 1) = 0.99999186;
  // // R(2, 1) = 0.00354355;

  // // R(0, 2) = 0.00058794;
  // // R(1, 2) = -0.00354457;
  // // R(2, 2) = 0.99999350;

  // // Vector3f t;

  // // t[0] = 0.00079127;
  // // t[1] = -0.00041675;
  // // t[2] = 0.01238126;

  // // const Transform Tinc = Transform::Translate(t) * Transform::Rotate(R);

  // frame.depth_to_world_transform = keyframe_->depth_to_world_transform;
  // tracker_->SetMaxIterations(1);
  // tracker_->SetTranslationEnabled(true);
  // tracker_->SetKeyframe(keyframe_);
  // tracker_->Track(frame);

  // // }
}

template class PyramidTracker<ColorTracker>;
template class PyramidTracker<DepthTracker>;
template class PyramidTracker<LightTracker>;

} // namespace vulcan