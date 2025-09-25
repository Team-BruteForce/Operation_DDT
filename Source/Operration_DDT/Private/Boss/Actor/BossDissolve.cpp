/**
 * @file BossDissolve.cpp
 * @brief 보스 디졸브 액터 구현 파일
 * 
 * @details
 * 보스의 디졸브 효과를 관리하는 액터입니다.
 * 보스 사망 시 디졸브 애니메이션을 처리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Actor/BossDissolve.h"

// Sets default values
ABossDissolve::ABossDissolve()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}