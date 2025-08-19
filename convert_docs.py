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

# 3. AI에게 포트폴리오용 블로그 스타일 문서 생성을 요청하는 프롬프트 만들기
def create_prompt(docs_data):
    prompt_content = """당신은 게임 개발과 소프트웨어 엔지니어링에 대한 전문적인 기술 블로그를 작성하는 개발자입니다. 

아래는 언리얼 엔진 C++ 프로젝트의 Doxygen에서 추출한 데이터입니다. 이 데이터를 바탕으로 개발자 포트폴리오나 기술 블로그에 적합한 전문적이고 상세한 기술 문서를 작성해주세요.

## 작성 요구사항:

### 1. 문서 스타일
- 기술 블로그나 개발자 포트폴리오에 적합한 전문적이고 읽기 쉬운 스타일
- 개발자들이 실제로 참고할 수 있는 실용적인 내용
- 코드 예시와 아키텍처 설명을 포함한 상세한 기술 분석

### 2. 문서 구조
- 프로젝트 개요 및 개발 배경
- 기술적 도전 과제와 해결 방법
- 시스템 아키텍처 및 설계 패턴
- 각 클래스/모듈의 상세 분석
- 성능 최적화 및 모범 사례
- 향후 개선 방향 및 확장성

### 3. 기술적 내용
- 언리얼 엔진의 고급 기능 활용 사례
- C++ 프로그래밍 패턴과 모범 사례
- 플러그인 아키텍처 및 모듈 시스템
- 실시간 데이터 동기화 시스템 설계
- AI 시스템 및 애니메이션 시스템 연동

### 4. 포트폴리오 요소
- 문제 해결 능력과 창의적 사고
- 대규모 시스템 설계 및 구현 경험
- 성능 최적화 및 코드 품질 관리
- 협업 및 프로젝트 관리 능력

## 작성 형식:
- 마크다운 형식으로 작성
- 코드 블록과 다이어그램 설명 포함
- 기술적 용어에 대한 명확한 설명
- 실제 개발 경험을 바탕으로 한 인사이트 제공

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### 클래스/파일: `{data['name']}`\n"
        prompt_content += f"**설명**: {data['brief']}\n"
        prompt_content += "**주요 함수**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    
    prompt_content += """

위 데이터를 바탕으로 언리얼 엔진 게임 개발 프로젝트에 대한 전문적인 기술 문서를 작성해주세요. 
실제 개발자들이 참고할 수 있고, 포트폴리오로 활용할 수 있는 수준의 상세하고 전문적인 내용으로 작성해주세요.
"""
    return prompt_content

# 4. Gemini API 호출 및 기술 문서 파일 작성
def generate_readme(prompt):
    print("Gemini API를 호출하여 포트폴리오용 기술 문서 생성을 시작합니다...")
    try:
        # 변경된 부분: Gemini 모델 초기화
        model = genai.GenerativeModel('gemini-1.5-pro-latest') 
        
        # 변경된 부분: 콘텐츠 생성 및 응답 처리
        response = model.generate_content(prompt)
        content = response.text

        # 포트폴리오용 기술 문서로 파일명 변경
        with open("TECHNICAL_PORTFOLIO.md", "w", encoding="utf-8") as f:
            f.write(content)
        print("TECHNICAL_PORTFOLIO.md 파일이 성공적으로 생성되었습니다!")
    except Exception as e:
        print(f"API 호출 중 오류 발생: {e}")

# 5. 메인 실행 로직
if __name__ == "__main__":
    doxygen_data = parse_doxygen_xml()
    if doxygen_data:
        final_prompt = create_prompt(doxygen_data)
        generate_readme(final_prompt)