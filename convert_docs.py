# convert_docs.py (Gemini API 버전)
import os
import google.generativeai as genai
from lxml import etree

# 1. 환경변수에서 Gemini API 키 가져오기
api_key = os.getenv("GEMINI_API_KEY") # 변경된 부분: 환경 변수 이름
if not api_key:
    raise ValueError("Gemini API 키가 설정되지 않았습니다. GEMINI_API_KEY 환경변수를 확인하세요.")

genai.configure(api_key=api_key) # 변경된 부분: API 키 설정 방식

# 2. Doxygen XML 파일에서 데이터 추출하기 (이 부분은 변경 없음)
def parse_doxygen_xml():
    try:
        tree = etree.parse('xml/index.xml')
        compounds = tree.xpath('//compound[@kind="class" or @kind="file"]')
        docs_data = []
        for compound in compounds:
            name = compound.find('name').text
            refid = compound.get('refid')
            detail_tree = etree.parse(f'xml/{refid}.xml')
            brief_description_node = detail_tree.find('.//briefdescription/para')
            brief = brief_description_node.text if brief_description_node is not None else "설명 없음"
            member_info = []
            members = detail_tree.xpath('.//memberdef[@kind="function"]')
            for member in members:
                member_name = member.find('name').text
                member_brief_node = member.find('briefdescription/para')
                member_brief = member_brief_node.text if member_brief_node is not None else ""
                member_info.append(f"- 함수 `{member_name}`: {member_brief}")
            docs_data.append({"name": name, "brief": brief, "members": member_info})
        return docs_data
    except FileNotFoundError:
        print("Doxygen XML 파일을 찾을 수 없습니다. Doxygen을 먼저 실행하세요.")
        return None

# 3. AI에게 README.md 생성을 요청하는 프롬프트 만들기
def create_prompt(docs_data):
    prompt_content = """당신은 게임 개발과 C++ 전문가이자 기술 문서 작성 전문가입니다. 아래는 Unreal Engine C++ 프로젝트의 Doxygen에서 추출한 데이터입니다. 이 데이터를 바탕으로 개발자 포트폴리오 수준의 매우 상세하고 전문적인 README.md 파일을 한국어로 작성해주세요.

## 중요 지침:
- **최소 15000단어 이상**의 극도로 상세한 문서를 작성해주세요
- **모든 클래스와 함수를 빠짐없이** 포함해주세요
- **실제 코드 예제와 구현 세부사항**을 중심으로 설명해주세요
- **노션 블로그처럼 이쁘게** 이모지와 마크다운을 활용해주세요
- **기능 중심으로** "아 이 기능을 만들었구나!"를 알 수 있게 작성해주세요
- **각 함수마다 상세한 설명과 예제**를 포함해주세요

## 요구사항:
1. **프로젝트 개요** (최소 2500단어):
   - 🎮 게임 장르, 배경, 목표
   - ⭐ 주요 기능과 특징을 매우 상세히 설명
   - 🛠️ 기술적 특징과 사용된 라이브러리/프레임워크
   - 📋 클래스 구조와 시스템 개요

2. **목차** (상세한 목차):
   - 모든 섹션과 서브섹션을 포함한 완전한 목차
   - 각 클래스별로 독립적인 섹션

3. **클래스별 상세 분석** (각 클래스당 최소 1500단어):
   - 🎯 클래스의 목적과 핵심 기능
   - 🔗 클래스 간 상호작용과 의존성
   - ⚙️ 주요 메서드와 프로퍼티 상세 설명
   - 💻 실제 사용 예제와 완전한 코드
   - 🚀 성능 특성과 최적화 포인트
   - 🔄 다른 클래스와의 상호작용

4. **시스템 아키텍처** (최소 2000단어):
   - 🏗️ 전체 시스템 구조 다이어그램 (Mermaid)
   - 🔗 클래스 간 관계와 의존성 상세 분석
   - 📊 데이터 플로우와 상태 관리
   - 🎨 설계 패턴과 아키텍처 결정
   - 🔄 시스템 생명주기와 초기화 과정

5. **핵심 기능 구현** (최소 3500단어):
   - 🧠 StateTree 기반 AI 시스템
   - ⚡ 페이즈 전환 시스템
   - 🎯 투사체 시스템과 오브젝트 풀링
   - 🎬 애니메이션 노티파이 시스템
   - 💫 이펙트 관리 시스템
   - 🎮 보스 상태 관리 시스템
   - 🔄 데이터 동기화 시스템
   - 📱 UI 연동 시스템
   - 🎵 사운드 관리 시스템
   - 🎨 렌더링 최적화

6. **고급 기능 및 최적화** (최소 2000단어):
   - 🚀 성능 최적화 기법
   - 💾 메모리 관리 전략
   - 🔧 디버깅 및 프로파일링
   - 📈 확장성 고려사항
   - 🛡️ 에러 처리 및 예외 상황

## 작성 스타일:
- 🎨 노션 블로그처럼 이쁘게 이모지와 마크다운 활용
- 💻 코드 중심으로 실제 구현 세부사항 강조
- 🎯 기능 중심으로 "이 기능을 만들었구나!"를 알 수 있게
- 📊 다이어그램과 시각적 자료 적극 활용
- 🔍 모든 클래스와 함수를 빠짐없이 포함
- 📝 실제 사용 가능한 완전한 코드 예제
- 🚀 기술적 깊이와 실무 경험 강조

## 특별 요구사항:
- 모든 클래스의 모든 함수를 상세히 설명
- 실제 코드 예제는 완전한 구현체로 작성
- 다이어그램은 Mermaid 형식으로 작성
- 이모지를 적절히 활용하여 가독성 향상
- 기능별로 명확하게 구분하여 설명
- 각 함수의 매개변수, 반환값, 예외 처리 상세 설명
- 실제 사용 시나리오와 베스트 프랙티스 포함
- 성능 특성과 최적화 포인트 명시
- 디버깅과 트러블슈팅 가이드 포함
- 확장성과 유지보수성 고려사항 포함

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### 클래스/파일: `{data['name']}`\n"
        prompt_content += f"**설명**: {data['brief']}\n"
        prompt_content += "**주요 함수**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    return prompt_content

# 추가 프롬프트 함수들
def create_api_docs_prompt(docs_data):
    """API 문서 생성용 프롬프트"""
    prompt_content = """당신은 Unreal Engine C++ API 문서 작성 전문가입니다. 아래 데이터를 바탕으로 개발자들이 실제 사용할 수 있는 매우 상세한 API 문서를 작성해주세요.

## 중요 지침:
- **최소 4000단어 이상**의 상세한 API 문서를 작성해주세요
- 모든 클래스와 함수에 대해 **완전한 문서화**를 제공해주세요
- 코드 예제는 **실제 프로젝트에서 사용 가능한 완전한 코드**로 작성해주세요

## 요구사항:
1. **클래스별 상세 설명** (각 클래스당 최소 500단어):
   - 클래스의 목적과 역할
   - 사용 시나리오와 예제
   - 상속 관계와 인터페이스
   - 주의사항과 제약사항
   - 성능 고려사항

2. **함수 시그니처** (각 함수당 최소 200단어):
   - 매개변수 상세 설명 (타입, 용도, 제약사항)
   - 반환값 설명 (타입, 의미, 가능한 값)
   - 예외 처리와 에러 케이스
   - 사용 예제와 베스트 프랙티스
   - 성능 특성과 최적화 팁

3. **사용 예제** (완전한 코드):
   - 실제 프로젝트에서 사용할 수 있는 완전한 예제
   - 다양한 사용 시나리오
   - 에러 처리와 예외 상황
   - 성능 최적화 예제

4. **성능 고려사항**:
   - 메모리 사용량과 할당 패턴
   - 실행 시간과 복잡도
   - 최적화 기법과 팁
   - 프로파일링 가이드

5. **호환성 정보**:
   - Unreal Engine 버전 요구사항
   - 플랫폼별 지원 정보
   - 컴파일러 요구사항
   - 의존성 정보

6. **트러블슈팅**:
   - 자주 발생하는 문제와 해결 방법
   - 디버깅 가이드
   - 성능 문제 해결
   - 메모리 누수 방지

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### {data['name']}\n"
        prompt_content += f"**설명**: {data['brief']}\n"
        prompt_content += "**API**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    return prompt_content

def create_architecture_prompt(docs_data):
    """아키텍처 분석용 프롬프트"""
    prompt_content = """당신은 소프트웨어 아키텍처 분석 전문가입니다. 아래 C++ 프로젝트 데이터를 분석하여 매우 상세한 시스템 아키텍처 문서를 작성해주세요.

## 중요 지침:
- **최소 5000단어 이상**의 상세한 아키텍처 문서를 작성해주세요
- 모든 시스템과 모듈에 대해 **깊이 있는 분석**을 제공해주세요
- 다이어그램과 시각적 자료를 **Mermaid 형식**으로 포함해주세요

## 분석 요구사항:
1. **시스템 전체 구조** (최소 1200단어):
   - 전체 시스템 아키텍처 다이어그램
   - 모듈 간 관계와 의존성 상세 분석
   - 데이터 플로우와 제어 플로우
   - 시스템 경계와 인터페이스
   - 레이어별 책임과 역할

2. **설계 패턴 식별** (최소 1000단어):
   - 사용된 디자인 패턴과 그 이유
   - 패턴 구현의 장단점 분석
   - 패턴 간 상호작용과 조합
   - 대안 패턴과 비교 분석
   - 패턴 적용의 효과성

3. **데이터 플로우** (최소 800단어):
   - 시스템 내 데이터 흐름 상세 분석
   - 상태 변화와 전환 과정
   - 데이터 변환과 처리 과정
   - 캐싱과 임시 저장 전략
   - 데이터 일관성과 동기화

4. **확장성 분석** (최소 800단어):
   - 시스템의 확장 가능성과 제약사항
   - 성능 병목 지점과 해결 방안
   - 수평적/수직적 확장 전략
   - 마이크로서비스 분리 가능성
   - 확장 시 고려사항

5. **성능 특성** (최소 600단어):
   - 병목 지점과 최적화 기회
   - 메모리 사용 패턴과 최적화
   - CPU 사용률과 병렬 처리
   - I/O 성능과 캐싱 전략
   - 성능 모니터링과 프로파일링

6. **유지보수성** (최소 600단어):
   - 코드 품질과 구조적 문제점
   - 리팩토링 제안과 개선 방안
   - 테스트 가능성과 커버리지
   - 문서화와 코드 가독성
   - 버전 관리와 배포 전략

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### {data['name']}\n"
        prompt_content += f"**역할**: {data['brief']}\n"
        prompt_content += "**주요 기능**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    return prompt_content


def create_portfolio_prompt(docs_data):
    """포트폴리오용 프롬프트"""
    prompt_content = """당신은 게임 개발 포트폴리오 작성 전문가입니다. 아래 프로젝트를 바탕으로 임팩트 있는 포트폴리오 문서를 작성해주세요.

## 중요 지침:
- **최소 4000단어 이상**의 상세한 포트폴리오 문서를 작성해주세요
- **기술적 깊이와 실무 경험**을 보여주는 내용으로 작성해주세요
- **구체적인 수치와 결과**를 포함해주세요

## 포트폴리오 구성:
1. **프로젝트 하이라이트** (최소 1000단어):
   - 가장 인상적인 기술적 성과 5가지
   - 각 성과의 기술적 난이도와 혁신성
   - 구체적인 구현 방법과 결과
   - 성능 개선 수치와 지표
   - 기술적 도전과 해결 과정

2. **기술적 도전과 해결** (최소 1200단어):
   - 복잡한 문제 해결 과정과 방법론
   - 알고리즘 설계와 최적화 과정
   - 성능 병목 해결과 최적화 결과
   - 메모리 관리와 리소스 최적화
   - 디버깅과 문제 해결 경험

3. **성능 지표** (최소 800단어):
   - 최적화 전후 성능 비교
   - 메모리 사용량 개선 결과
   - CPU 사용률 최적화 결과
   - 프레임레이트 개선 수치
   - 로딩 시간 단축 결과

4. **학습 성과** (최소 600단어):
   - 새로운 기술 습득과 적용
   - 아키텍처 설계 경험
   - 디자인 패턴 활용
   - 최신 기술 트렌드 적용
   - 지속적인 학습과 성장

5. **협업 경험** (최소 400단어):
   - 팀 프로젝트에서의 역할과 기여도
   - 코드 리뷰와 협업 경험
   - 프로젝트 관리와 일정 관리
   - 커뮤니케이션과 리더십
   - 지식 공유와 멘토링

6. **향후 발전 방향** (최소 400단어):
   - 개선 계획과 로드맵
   - 새로운 기술 학습 계획
   - 프로젝트 확장 방향
   - 기술적 목표와 비전
   - 커리어 발전 계획

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### {data['name']}\n"
        prompt_content += f"**기능**: {data['brief']}\n"
        prompt_content += "**주요 구현**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    return prompt_content


def create_class_analysis_prompt(docs_data):
    """클래스 분석용 프롬프트"""
    prompt_content = """당신은 C++ 클래스 설계 및 분석 전문가입니다. 아래 데이터를 바탕으로 각 클래스의 기능과 역할을 중심으로 매우 상세한 분석 문서를 작성해주세요.

## 중요 지침:
- **최소 6000단어 이상**의 상세한 클래스 분석 문서를 작성해주세요
- **각 클래스별로 독립적인 섹션**을 만들어 상세히 분석해주세요
- **실제 코드 예제와 사용법**을 포함해주세요

## 분석 요구사항:
1. **클래스 개요** (전체 1000단어):
   - 프로젝트 내 클래스들의 전체적인 역할
   - 클래스 간 관계와 의존성
   - 설계 철학과 아키텍처 패턴

2. **핵심 클래스 분석** (각 클래스당 최소 800단어):
   - 클래스의 목적과 책임
   - 주요 메서드와 프로퍼티 상세 분석
   - 클래스의 생명주기와 상태 관리
   - 실제 사용 예제와 코드
   - 성능 특성과 최적화 포인트
   - 사용 시나리오와 베스트 프랙티스
   - 다른 클래스와의 상호작용

3. **설계 패턴 분석** (최소 1000단어):
   - 사용된 디자인 패턴과 그 이유
   - 패턴 구현의 장단점
   - 대안 패턴과 비교
   - 패턴 적용의 효과성

4. **성능 및 최적화** (최소 800단어):
   - 각 클래스의 성능 특성
   - 메모리 사용 패턴
   - 최적화 기법과 결과
   - 병목 지점과 해결 방안

5. **확장성 및 유지보수성** (최소 600단어):
   - 클래스의 확장 가능성
   - 코드 품질과 가독성
   - 테스트 가능성
   - 리팩토링 제안

6. **실무 적용 가이드** (최소 800단어):
   - 실제 프로젝트에서의 사용법
   - 주의사항과 제약사항
   - 문제 해결 가이드
   - 베스트 프랙티스

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### 클래스: `{data['name']}`\n"
        prompt_content += f"**기능**: {data['brief']}\n"
        prompt_content += "**주요 메서드**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    return prompt_content

def create_implementation_guide_prompt(docs_data):
    """구현 가이드용 프롬프트"""
    prompt_content = """당신은 게임 개발 구현 가이드 작성 전문가입니다. 아래 데이터를 바탕으로 실제 개발자가 따라할 수 있는 상세한 구현 가이드를 작성해주세요.

## 중요 지침:
- **최소 5000단어 이상**의 실용적인 구현 가이드를 작성해주세요
- **단계별 구현 과정**을 상세히 설명해주세요
- **완전한 코드 예제**를 포함해주세요

## 가이드 구성:
1. **프로젝트 설정** (최소 800단어):
   - 개발 환경 구성
   - 프로젝트 구조 설정
   - 의존성 관리
   - 빌드 설정

2. **핵심 클래스 구현** (각 클래스당 최소 1000단어):
   - 클래스 설계 및 구현
   - 주요 메서드 구현
   - 프로퍼티 설정
   - 초기화 및 정리
   - 실제 사용 예제

3. **시스템 통합** (최소 1200단어):
   - 클래스 간 연동
   - 데이터 플로우 구현
   - 이벤트 시스템 구성
   - 상태 관리 구현

4. **최적화 및 성능 튜닝** (최소 800단어):
   - 성능 프로파일링
   - 메모리 최적화
   - CPU 최적화
   - 렌더링 최적화

5. **테스트 및 디버깅** (최소 600단어):
   - 단위 테스트 작성
   - 통합 테스트
   - 디버깅 기법
   - 성능 테스트

6. **배포 및 유지보수** (최소 600단어):
   - 빌드 및 패키징
   - 배포 전략
   - 버전 관리
   - 유지보수 가이드

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### 구현 대상: `{data['name']}`\n"
        prompt_content += f"**설명**: {data['brief']}\n"
        prompt_content += "**구현 요소**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    return prompt_content

# 4. Gemini API 호출 및 문서 파일 작성 (API 호출 부분 전체 변경)
def generate_readme(prompt):
    print("Gemini API를 호출하여 README.md 생성을 시작합니다...")
    try:
        # 변경된 부분: Gemini 모델 초기화
        model = genai.GenerativeModel('gemini-1.5-pro-latest') 
        
        # 변경된 부분: 콘텐츠 생성 및 응답 처리
        response = model.generate_content(prompt)
        content = response.text

        with open("README.md", "w", encoding="utf-8") as f:
            f.write(content)
        print("README.md 파일이 성공적으로 생성되었습니다!")
    except Exception as e:
        print(f"API 호출 중 오류 발생: {e}")

# 5. 메인 실행 로직
def main():
    doxygen_data = parse_doxygen_xml()
    if not doxygen_data:
        return
    
    # README만 생성
    print("README.md만 생성합니다...")
    final_prompt = create_prompt(doxygen_data)
    generate_document(final_prompt, "README.md")
    
    print("\nREADME.md 생성이 완료되었습니다!")

def generate_document(prompt, filename):
    """문서 생성 함수"""
    print(f"Gemini API를 호출하여 {filename} 생성을 시작합니다...")
    print(f"프롬프트 길이: {len(prompt)} 문자")
    
    try:
        # Gemini 2.0 Flash 모델 사용 (더 많은 토큰 지원)
        model = genai.GenerativeModel('gemini-2.0-flash-exp')
        
        # 최대한 많은 토큰으로 설정
        generation_config = genai.types.GenerationConfig(
            max_output_tokens=8192,   # 최대 출력 토큰 (8K)
            temperature=0.7,          # 창의성과 일관성의 균형
            top_p=0.8,                # 토큰 선택 범위
            top_k=40,                 # 상위 k개 토큰에서 선택
            candidate_count=1         # 후보 수
        )
        
        # 안전 설정 (필터링 최소화)
        safety_settings = [
            {
                "category": "HARM_CATEGORY_HARASSMENT",
                "threshold": "BLOCK_MEDIUM_AND_ABOVE"
            },
            {
                "category": "HARM_CATEGORY_HATE_SPEECH", 
                "threshold": "BLOCK_MEDIUM_AND_ABOVE"
            },
            {
                "category": "HARM_CATEGORY_SEXUALLY_EXPLICIT",
                "threshold": "BLOCK_MEDIUM_AND_ABOVE"
            },
            {
                "category": "HARM_CATEGORY_DANGEROUS_CONTENT",
                "threshold": "BLOCK_MEDIUM_AND_ABOVE"
            }
        ]
        
        response = model.generate_content(
            prompt, 
            generation_config=generation_config,
            safety_settings=safety_settings
        )
        
        content = response.text

        with open(filename, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"{filename} 파일이 성공적으로 생성되었습니다!")
        print(f"생성된 내용 길이: {len(content)} 문자")
        print(f"예상 단어 수: 약 {len(content.split())} 단어")
        
    except Exception as e:
        print(f"API 호출 중 오류 발생: {e}")
        print(f"오류 상세: {str(e)}")
        
        # 오류가 발생하면 더 작은 토큰으로 재시도
        print("더 작은 토큰 수로 재시도합니다..")
        try:
            generation_config = genai.types.GenerationConfig(
                max_output_tokens=4096,   # 4K 토큰으로 재시도
                temperature=0.7
            )
            response = model.generate_content(prompt, generation_config=generation_config)
            content = response.text
            
            with open(filename, "w", encoding="utf-8") as f:
                f.write(content)
            print(f"{filename} 파일이 재시도로 생성되었습니다!")
            print(f"생성된 내용 길이: {len(content)} 문자")
            
        except Exception as e2:
            print(f"재시도도 실패했습니다: {e2}")

if __name__ == "__main__":
    main()