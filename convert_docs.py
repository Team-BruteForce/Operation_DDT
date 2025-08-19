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

# 3. AI에게 README.md 생성을 요청하는 프롬프트 만들기 (이 부분은 변경 없음)
def create_prompt(docs_data):
    prompt_content = """당신은 코드 문서를 아주 잘 작성하는 전문가입니다. 아래는 C++ 프로젝트의 Doxygen에서 추출한 데이터입니다. 이 데이터를 바탕으로 GitHub 사용자들이 이해하기 쉬운 멋진 README.md 파일을 한국어로 작성해주세요.

각 클래스와 주요 함수에 대해 설명하고, 코드 블록을 적절히 사용해서 예쁘게 꾸며주세요. 프로젝트의 전반적인 소개로 시작해주세요.

[추출된 문서 데이터]
"""
    for data in docs_data:
        prompt_content += f"\n### 클래스/파일: `{data['name']}`\n"
        prompt_content += f"**설명**: {data['brief']}\n"
        prompt_content += "**주요 함수**:\n"
        for member in data['members']:
            prompt_content += f"{member}\n"
    return prompt_content

# 4. Gemini API 호출 및 README.md 파일 작성 (API 호출 부분 전체 변경)
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

# 5. 메인 실행 로직 (이 부분은 변경 없음)
if __name__ == "__main__":
    doxygen_data = parse_doxygen_xml()
    if doxygen_data:
        final_prompt = create_prompt(doxygen_data)
        generate_readme(final_prompt)