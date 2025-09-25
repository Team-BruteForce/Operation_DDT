# convert_docs.py (Gemini API 버전)
import os
import google.generativeai as genai # 변경된 부분: 라이브러리 변경
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
    prompt_content = """당신은 게임 개발과 C++ 전문가이자 기술 문서 작성 전문가입니다. 아래는 Unreal Engine C++ 프로젝트의 Doxygen에서 추출한 데이터입니다. 이 데이터를 바탕으로 개발자 포트폴리오 수준의 전문적인 README.md 파일을 한국어로 작성해주세요.

## 요구사항:
1. **프로젝트 개요**: 게임의 장르, 주요 기능, 기술적 특징을 명확히 설명
2. **아키텍처 분석**: 클래스 간 관계, 설계 패턴, 시스템 구조를 다이어그램과 함께 설명
3. **핵심 시스템 설명**: 각 컴포넌트의 역할과 상호작용을 상세히 기술
4. **API 문서**: 주요 클래스와 함수의 사용법과 예제 코드 포함
5. **기술적 하이라이트**: 사용된 알고리즘, 최적화 기법, 특별한 구현 방식 강조
6. **설치 및 실행 가이드**: 개발 환경 설정부터 빌드까지 단계별 설명
7. **개발자 노트**: 설계 결정 이유, 트레이드오프, 향후 개선 계획 포함

## 작성 스타일:
- 전문적이면서도 이해하기 쉬운 문체
- 코드 블록과 이모지를 적절히 활용
- 섹션별로 명확한 구조화
- 개발자 관점에서 실용적인 정보 제공

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
    prompt_content = """당신은 Unreal Engine C++ API 문서 작성 전문가입니다. 아래 데이터를 바탕으로 개발자들이 실제 사용할 수 있는 상세한 API 문서를 작성해주세요.

## 요구사항:
1. **클래스별 상세 설명**: 각 클래스의 목적, 사용 시나리오, 주의사항
2. **함수 시그니처**: 매개변수, 반환값, 예외 처리 상세 설명
3. **사용 예제**: 실제 코드 예제와 함께 설명
4. **성능 고려사항**: 메모리 사용량, 실행 시간, 최적화 팁
5. **호환성 정보**: Unreal Engine 버전, 플랫폼 지원 정보
6. **트러블슈팅**: 자주 발생하는 문제와 해결 방법

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
    prompt_content = """당신은 소프트웨어 아키텍처 분석 전문가입니다. 아래 C++ 프로젝트 데이터를 분석하여 시스템 아키텍처 문서를 작성해주세요.

## 분석 요구사항:
1. **시스템 전체 구조**: 모듈 간 관계와 의존성 분석
2. **설계 패턴 식별**: 사용된 디자인 패턴과 그 이유
3. **데이터 플로우**: 시스템 내 데이터 흐름과 상태 변화
4. **확장성 분석**: 시스템의 확장 가능성과 제약사항
5. **성능 특성**: 병목 지점과 최적화 기회
6. **유지보수성**: 코드 품질과 리팩토링 제안

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

## 포트폴리오 구성:
1. **프로젝트 하이라이트**: 가장 인상적인 기술적 성과 3가지
2. **기술적 도전과 해결**: 복잡한 문제 해결 과정과 방법론
3. **성능 지표**: 최적화 결과, 성능 개선 수치
4. **학습 성과**: 새로운 기술 습득, 아키텍처 설계 경험
5. **협업 경험**: 팀 프로젝트에서의 역할과 기여도
6. **향후 발전 방향**: 개선 계획과 학습 목표

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### {data['name']}\n"
        prompt_content += f"**기능**: {data['brief']}\n"
        prompt_content += "**주요 구현**:\n"
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
    
    # 모든 문서 생성
    prompts = [
        ("README.md", create_prompt),
        ("API_DOCS.md", create_api_docs_prompt),
        ("ARCHITECTURE.md", create_architecture_prompt),
        ("PORTFOLIO.md", create_portfolio_prompt)
    ]
    
    print("모든 문서를 생성합니다...")
    for filename, prompt_func in prompts:
        print(f"\n{filename} 생성 중...")
        final_prompt = prompt_func(doxygen_data)
        generate_document(final_prompt, filename)
    
    print("\n모든 문서 생성이 완료되었습니다!")

def generate_document(prompt, filename):
    """문서 생성 함수"""
    print(f"Gemini API를 호출하여 {filename} 생성을 시작합니다...")
    try:
        model = genai.GenerativeModel('gemini-1.5-pro-latest')
        response = model.generate_content(prompt)
        content = response.text

        with open(filename, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"{filename} 파일이 성공적으로 생성되었습니다!")
    except Exception as e:
        print(f"API 호출 중 오류 발생: {e}")

if __name__ == "__main__":
    main()