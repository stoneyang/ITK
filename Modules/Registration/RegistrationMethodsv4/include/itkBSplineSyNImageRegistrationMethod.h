/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkBSplineSyNImageRegistrationMethod_h
#define itkBSplineSyNImageRegistrationMethod_h

#include "itkSyNImageRegistrationMethod.h"

#include "itkBSplineSmoothingOnUpdateDisplacementFieldTransform.h"
#include "itkImageMaskSpatialObject.h"

namespace itk
{

/** \class BSplineSyNImageRegistrationMethod
 * \brief Interface method for the performing greedy B-spline SyN image registration.
 *
 * For greedy SyN we use \c m_Transform to map the time-parameterized middle
 * image to the fixed image (and vice versa using
 * \c m_Transform->GetInverseDisplacementField() ).  We employ another ivar,
 * \c m_InverseTransform, to map the time-parameterized middle image to the
 * moving image.
 *
 * Output: The output is the updated transform which has been added to the
 * composite transform.
 *
 * \author Nick Tustison
 * \author Brian Avants
 *
 * \ingroup ITKRegistrationMethodsv4
 */
template<typename TFixedImage, typename TMovingImage, typename TOutputTransform =
  BSplineSmoothingOnUpdateDisplacementFieldTransform<double, TFixedImage::ImageDimension> >
class BSplineSyNImageRegistrationMethod
: public SyNImageRegistrationMethod<TFixedImage, TMovingImage, TOutputTransform>
{
public:
  /** Standard class typedefs. */
  typedef BSplineSyNImageRegistrationMethod                                           Self;
  typedef SyNImageRegistrationMethod<TFixedImage, TMovingImage, TOutputTransform>     Superclass;
  typedef SmartPointer<Self>                                                          Pointer;
  typedef SmartPointer<const Self>                                                    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** ImageDimension constants */
  itkStaticConstMacro( ImageDimension, unsigned int, TFixedImage::ImageDimension );

  /** Run-time type information (and related methods). */
  itkTypeMacro( BSplineSyNImageRegistrationMethod, SyNImageRegistrationMethod );

  /** Input typedefs for the images. */
  typedef TFixedImage                                                 FixedImageType;
  typedef typename FixedImageType::Pointer                            FixedImagePointer;
  typedef typename Superclass::FixedImagesContainerType               FixedImagesContainerType;
  typedef TMovingImage                                                MovingImageType;
  typedef typename MovingImageType::Pointer                           MovingImagePointer;
  typedef typename Superclass::MovingImagesContainerType              MovingImagesContainerType;

  typedef typename Superclass::PointSetType                           PointSetType;

  /** Metric and transform typedefs */
  typedef typename Superclass::ImageMetricType                        ImageMetricType;
  typedef typename ImageMetricType::Pointer                           ImageMetricPointer;
  typedef typename ImageMetricType::VirtualImageType                  VirtualImageType;
  typedef typename ImageMetricType::MeasureType                       MeasureType;
  typedef typename ImageMetricType::FixedImageMaskType                FixedImageMaskType;
  typedef typename ImageMetricType::MovingImageMaskType               MovingImageMaskType;

  typedef typename Superclass::MultiMetricType                        MultiMetricType;
  typedef typename Superclass::MetricType                             MetricType;
  typedef typename MetricType::Pointer                                MetricPointer;
  typedef typename Superclass::PointSetMetricType                     PointSetMetricType;

  typedef typename Superclass::NumberOfIterationsArrayType            NumberOfIterationsArrayType;

  typedef typename Superclass::InitialTransformType                   InitialTransformType;
  typedef TOutputTransform                                            OutputTransformType;
  typedef typename OutputTransformType::Pointer                       OutputTransformPointer;
  typedef typename OutputTransformType::ScalarType                    RealType;
  typedef typename OutputTransformType::DerivativeType                DerivativeType;
  typedef typename DerivativeType::ValueType                          DerivativeValueType;
  typedef typename OutputTransformType::DisplacementFieldType         DisplacementFieldType;
  typedef typename DisplacementFieldType::Pointer                     DisplacementFieldPointer;
  typedef typename DisplacementFieldType::PixelType                   DisplacementVectorType;

  typedef DisplacementFieldToBSplineImageFilter<DisplacementFieldType>  BSplineFilterType;
  typedef typename BSplineFilterType::ArrayType                         ArrayType;
  typedef typename BSplineFilterType::RealImageType                     WeightedMaskImageType;

  typedef ImageMaskSpatialObject<ImageDimension>                      ImageMaskSpatialObjectType;
  typedef typename ImageMaskSpatialObjectType::ImageType              MaskImageType;

  typedef typename Superclass::CompositeTransformType                 CompositeTransformType;
  typedef typename CompositeTransformType::TransformType              TransformBaseType;

  typedef typename Superclass::DecoratedOutputTransformType           DecoratedOutputTransformType;
  typedef typename DecoratedOutputTransformType::Pointer              DecoratedOutputTransformPointer;

protected:
  BSplineSyNImageRegistrationMethod();
  virtual ~BSplineSyNImageRegistrationMethod();

  /** Handle optimization internally */
  virtual void StartOptimization() ITK_OVERRIDE;

  virtual void InitializeRegistrationAtEachLevel( const SizeValueType ) ITK_OVERRIDE;

  virtual DisplacementFieldPointer ComputeUpdateField( const FixedImagesContainerType, const TransformBaseType *,
    const MovingImagesContainerType, const TransformBaseType *, const FixedImageMaskType *, MeasureType & ) ITK_OVERRIDE;
  virtual DisplacementFieldPointer BSplineSmoothDisplacementField( const DisplacementFieldType *, const ArrayType &, const WeightedMaskImageType * );

private:
  BSplineSyNImageRegistrationMethod( const Self & );   //purposely not implemented
  void operator=( const Self & );               //purposely not implemented
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBSplineSyNImageRegistrationMethod.hxx"
#endif

#endif
