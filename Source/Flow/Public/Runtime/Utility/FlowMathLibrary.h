#pragma once

#include "DSP/Granulator.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FlowMathLibrary.generated.h"

UCLASS()
class FLOW_API UFlowMathLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
public:
	static constexpr auto CounterClockwiseRotationAngleThreshold{ 5.0f };

public:

	/*
	 *	FRAME RATE INDEPENDENT DAMPING USING LERP
	 *	指数衰减角
	 */

	UFUNCTION(BlueprintPure, Category = "Math")
	static float ExponentialDecayAngle(float Current, float Target, float DeltaTime, float Lambda);

	UFUNCTION(BlueprintPure, Category = "Math")
	static float LerpAngle(float From, float To, float Alpha);

	/*
	 *	理解为FInterpTo 类型
	 *
	 *	详见 https://www.rorydriscoll.com/2016/03/07/frame-rate-independent-damping-using-lerp/
	 *
	 *	指数书
	 *	FRAME RATE INDEPENDENT DAMPING USING LERP
	 *	按速率平滑过渡从A过渡到0（曲线）
	 *	a(t+n) = a(t) * r^n;
	 *
	 *	按速率从A平滑过渡到B（曲线）
	 *
	 *	a(t+n) - b = (a(t)-b) * r^n;
	 *	-> a(t+n) = b + (a(t)-b) * r^n;
	 *	-> a(t+n) = b(1-r^n) + a(t) * r^n;
	 *
	 *	和指数衰减函数一致 -> 直接用最后的公式即可
	 *
	 *	Lambda -> 0到正无穷。
	 */

	UFUNCTION(BlueprintPure, Category = "ALS|Als Math")
	static float ExponentialDecay(float DeltaTime, float Lambda);

	/*
	 * 理解为FInterpToConst类型
	 */
	static float InterpolateAngleConstant(const float Current, const float Target, const float DeltaTime, const float InterpolationSpeed);

};

inline float UFlowMathLibrary::ExponentialDecayAngle(float Current, float Target, float DeltaTime, float Lambda)
{
	return Lambda > 0.0f
		? LerpAngle(Current, Target, ExponentialDecay(DeltaTime, Lambda))
		: Target;
}

inline float UFlowMathLibrary::LerpAngle(float From, float To, float Alpha)
{
	auto Delta{ FRotator3f::NormalizeAxis(To - From) };

	if (Delta > 180.0f - CounterClockwiseRotationAngleThreshold)
	{
		Delta -= 360.0f;
	}

	return FRotator3f::NormalizeAxis(From + Delta * Alpha);
}

inline float UFlowMathLibrary::ExponentialDecay(float DeltaTime, float Lambda)
{
	//详见 https ://www.rorydriscoll.com/2016/03/07/frame-rate-independent-damping-using-lerp/

	return 1.0f - FMath::InvExpApprox(Lambda * DeltaTime);
}

inline float UFlowMathLibrary::InterpolateAngleConstant(const float Current, const float Target, const float DeltaTime,
	const float InterpolationSpeed)
{
	if (InterpolationSpeed <= 0.0f || Current == Target)
	{
		return Target;
	}

	auto Delta{ FRotator3f::NormalizeAxis(Target - Current) };

	if (Delta > 180.0f - CounterClockwiseRotationAngleThreshold)
	{
		Delta -= 360.0f;
	}

	const auto Alpha{ InterpolationSpeed * DeltaTime };

	return FRotator3f::NormalizeAxis(Current + FMath::Clamp(Delta, -Alpha, Alpha));
}
