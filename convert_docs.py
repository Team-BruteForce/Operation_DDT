# convert_docs_new.py - Boss 클래스별 API 분석 및 문서 생성
import os
import google.generativeai as genai
import glob
import re
from datetime import datetime
import json

# 1. 환경변수에서 Gemini API 키 가져오기
api_key = os.getenv("GEMINI_API_KEY")
if not api_key:
    raise ValueError("Gemini API 키가 설정되지 않았습니다. GEMINI_API_KEY 환경변수를 확인하세요.")

genai.configure(api_key=api_key)

# 2. Boss 폴더의 모든 클래스 파일들을 찾기
def find_boss_classes():
    """Boss 폴더의 모든 .h 파일을 찾아서 클래스 정보 추출"""
    boss_public_path = "Source/Operration_DDT/Public/Boss"
    boss_private_path = "Source/Operration_DDT/Private/Boss"
    
    # .h 파일들 찾기
    h_files = glob.glob(f"{boss_public_path}/**/*.h", recursive=True)
    h_files.extend(glob.glob(f"{boss_public_path}/*.h", recursive=False))
    
    classes = []
    
    for h_file in h_files:
        try:
            with open(h_file, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # 클래스명 추출
            class_matches = re.findall(r'class\s+(\w+)\s*:', content)
            if class_matches:
                class_name = class_matches[0]
                
                # 파일 경로에서 카테고리 추출
                relative_path = h_file.replace(f"{boss_public_path}/", "")
                category = relative_path.split('/')[0] if '/' in relative_path else "Root"
                
                # 함수들 추출
                functions = re.findall(r'(?:UFUNCTION|virtual|static)?\s*\w+\s+(\w+)\s*\([^)]*\)', content)
                
                # 멤버 변수들 추출
                members = re.findall(r'(?:UPROPERTY|protected|private)?\s*\w+\s+(\w+)\s*;', content)
                
                classes.append({
                    'name': class_name,
                    'file': h_file,
                    'category': category,
                    'functions': functions[:10],  # 최대 10개만
                    'members': members[:10],     # 최대 10개만
                    'content_preview': content[:500]  # 처음 500자만
                })
                
        except Exception as e:
            print(f"파일 읽기 오류 {h_file}: {e}")
    
    return classes

# 3. Doxygen 주석 분석 함수
def analyze_doxygen_comments(file_path):
    """Doxygen 주석을 분석하여 더 정확한 정보 추출"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Doxygen 주석 패턴들
        brief_pattern = r'@brief\s+(.+?)(?=\n|\*|@)'
        param_pattern = r'@param\s+(\w+)\s+(.+?)(?=\n|\*|@)'
        return_pattern = r'@return\s+(.+?)(?=\n|\*|@)'
        see_pattern = r'@see\s+(.+?)(?=\n|\*|@)'
        note_pattern = r'@note\s+(.+?)(?=\n|\*|@)'
        warning_pattern = r'@warning\s+(.+?)(?=\n|\*|@)'
        
        # 주석에서 정보 추출
        brief = re.findall(brief_pattern, content)
        params = re.findall(param_pattern, content)
        returns = re.findall(return_pattern, content)
        sees = re.findall(see_pattern, content)
        notes = re.findall(note_pattern, content)
        warnings = re.findall(warning_pattern, content)
        
        return {
            'brief': brief,
            'params': params,
            'returns': returns,
            'sees': sees,
            'notes': notes,
            'warnings': warnings
        }
    except Exception as e:
        print(f"Doxygen 주석 분석 오류 {file_path}: {e}")
        return {'brief': [], 'params': [], 'returns': [], 'sees': [], 'notes': [], 'warnings': []}

# 4. 클래스별 API 분석 함수
def analyze_class_with_api(class_info):
    """개별 클래스에 대해 API 분석 (Doxygen 주석 포함)"""
    # Doxygen 주석 분석
    doxygen_info = analyze_doxygen_comments(class_info['file'])
    
    prompt = f"""
당신은 Unreal Engine C++ 전문가입니다. 아래 클래스를 분석하여 상세한 API 문서를 작성해주세요.

## 클래스 정보:
- **클래스명**: {class_info['name']}
- **카테고리**: {class_info['category']}
- **파일 경로**: {class_info['file']}

## Doxygen 주석 정보:
### Brief 설명:
{chr(10).join([f"- {desc}" for desc in doxygen_info['brief']]) if doxygen_info['brief'] else "설명 없음"}

### 매개변수 정보:
{chr(10).join([f"- {param[0]}: {param[1]}" for param in doxygen_info['params']]) if doxygen_info['params'] else "매개변수 정보 없음"}

### 반환값 정보:
{chr(10).join([f"- {ret}" for ret in doxygen_info['returns']]) if doxygen_info['returns'] else "반환값 정보 없음"}

### 관련 클래스:
{chr(10).join([f"- {see}" for see in doxygen_info['sees']]) if doxygen_info['sees'] else "관련 클래스 정보 없음"}

### 주의사항:
{chr(10).join([f"- {note}" for note in doxygen_info['notes']]) if doxygen_info['notes'] else "주의사항 없음"}

### 경고사항:
{chr(10).join([f"- {warning}" for warning in doxygen_info['warnings']]) if doxygen_info['warnings'] else "경고사항 없음"}

## 클래스 코드 미리보기:
```cpp
{class_info['content_preview']}
```

## 주요 함수들:
{chr(10).join([f"- {func}" for func in class_info['functions']])}

## 주요 멤버 변수들:
{chr(10).join([f"- {member}" for member in class_info['members']])}

## 요구사항:
1. **Doxygen 주석을 기반으로** 클래스의 목적과 역할을 명확히 설명
2. **주요 함수들의 기능**을 상세히 설명 (매개변수, 반환값 포함)
3. **사용 시나리오와 예제**를 포함
4. **주의사항과 제약사항**을 명시
5. **다른 클래스와의 관계**를 설명

## 출력 형식:
```markdown
# {class_info['name']}

## 개요
[클래스의 목적과 역할 - Doxygen 주석 기반]

## 주요 기능
[주요 함수들의 기능 설명 - 매개변수와 반환값 포함]

## 사용 예제
[실제 사용 예제]

## 주의사항
[사용 시 주의할 점 - Doxygen 주석의 warning/note 기반]

## 관련 클래스
[다른 클래스와의 관계 - @see 기반]
```
"""
    
    try:
        model = genai.GenerativeModel('gemini-1.5-pro-latest')
        response = model.generate_content(prompt)
        return response.text
    except Exception as e:
        print(f"API 호출 오류 ({class_info['name']}): {e}")
        return f"# {class_info['name']}\n\nAPI 분석 중 오류 발생: {e}"

# 4. 문서 생성 함수들
def create_api_docs(classes_analysis):
    """API 문서 생성"""
    prompt = f"""
당신은 Unreal Engine C++ API 문서 작성 전문가입니다. 
아래 분석된 클래스들을 바탕으로 완전한 API 문서를 작성해주세요.

## 분석된 클래스들:
{chr(10).join([f"- {cls['name']} ({cls['category']})" for cls in classes_analysis])}

## 요구사항:
1. **전체 시스템 개요**를 먼저 설명
2. **카테고리별로 클래스들을 그룹화**하여 설명
3. **각 클래스의 핵심 기능**을 요약
4. **클래스 간의 관계와 의존성**을 설명
5. **사용 가이드와 베스트 프랙티스**를 포함

## 출력 형식:
```markdown
# Boss AI 시스템 API 문서

## 시스템 개요
[전체 시스템에 대한 설명]

## 클래스 카테고리별 분석
### 1. 핵심 클래스
[CBoss, CBossAIC 등]

### 2. 컴포넌트 클래스
[각종 Component 클래스들]

### 3. StateTree 관련
[StateTree, Task, Condition 클래스들]

### 4. 이펙트 시스템
[Effect 관련 클래스들]

### 5. 애니메이션 노티파이
[Notifies 클래스들]

## 사용 가이드
[실제 사용 방법]

## 베스트 프랙티스
[개발 시 주의사항]
```
"""
    
    try:
        model = genai.GenerativeModel('gemini-1.5-pro-latest')
        response = model.generate_content(prompt)
        return response.text
    except Exception as e:
        return f"API 문서 생성 오류: {e}"

def create_architecture_docs(classes_analysis):
    """아키텍처 문서 생성"""
    prompt = f"""
당신은 소프트웨어 아키텍처 분석 전문가입니다.
아래 클래스들을 바탕으로 시스템 아키텍처 문서를 작성해주세요.

## 분석된 클래스들:
{chr(10).join([f"- {cls['name']} ({cls['category']})" for cls in classes_analysis])}

## 요구사항:
1. **시스템 전체 구조**를 다이어그램으로 설명
2. **설계 패턴**을 식별하고 분석
3. **데이터 플로우**를 설명
4. **확장성과 유지보수성**을 분석
5. **성능 특성**을 설명

## 출력 형식:
```markdown
# Boss AI 시스템 아키텍처

## 시스템 개요
[전체 아키텍처 설명]

## 설계 패턴
[사용된 디자인 패턴들]

## 클래스 다이어그램
[시스템 구조도]

## 데이터 플로우
[데이터 흐름 설명]

## 확장성 분석
[확장 가능성과 제약사항]

## 성능 특성
[성능 최적화 포인트]
```
"""
    
    try:
        model = genai.GenerativeModel('gemini-1.5-pro-latest')
        response = model.generate_content(prompt)
        return response.text
    except Exception as e:
        return f"아키텍처 문서 생성 오류: {e}"

def create_readme_docs(classes_analysis):
    """README 문서 생성"""
    prompt = f"""
당신은 게임 개발 기술 블로그 작가입니다.
아래 프로젝트를 바탕으로 기술 블로그 스타일의 README를 작성해주세요.

## 프로젝트 정보:
- **프로젝트명**: Operation DDT - Boss AI 시스템
- **언어**: Unreal Engine C++
- **주요 기능**: StateTree 기반 보스 AI, 페이즈 전환, 투사체 시스템, 이펙트 관리
- **클래스 수**: {len(classes_analysis)}개

## 분석된 클래스들:
{chr(10).join([f"- {cls['name']} ({cls['category']})" for cls in classes_analysis])}

## 요구사항:
1. **프로젝트 소개**와 목적
2. **주요 기능들**을 카테고리별로 설명
3. **기술 스택**과 아키텍처 특징
4. **개발 과정**과 도전 과제
5. **향후 계획**과 발전 방향

## 작성 스타일:
- 기술 블로그처럼 읽기 쉽게
- 이모지와 마크다운 활용
- 전문적이지만 접근하기 쉬운 문체
- 구체적인 수치와 결과 포함

## 출력 형식:
```markdown
# ⚔️ Operation DDT - Boss AI 시스템

## 🎮 프로젝트 소개
[프로젝트 개요]

## ⚙️ 주요 기능
[핵심 기능들]

## 🛠️ 기술 스택
[사용된 기술들]

## 🏗️ 시스템 구조
[아키텍처 설명]

## 🧑‍💻 개발 과정
[개발 경험과 도전]

## 🚀 향후 계획
[발전 방향]
```
"""
    
    try:
        model = genai.GenerativeModel('gemini-1.5-pro-latest')
        response = model.generate_content(prompt)
        return response.text
    except Exception as e:
        return f"README 문서 생성 오류: {e}"

# 5. 메인 실행 함수
def main():
    print("🔍 Boss 클래스들을 분석 중...")
    classes = find_boss_classes()
    print(f"📊 총 {len(classes)}개의 클래스를 발견했습니다!")
    
    # 클래스별로 API 분석
    print("\n🤖 각 클래스별 API 분석을 시작합니다...")
    classes_analysis = []
    
    for i, class_info in enumerate(classes, 1):
        print(f"[{i}/{len(classes)}] {class_info['name']} 분석 중...")
        analysis = analyze_class_with_api(class_info)
        classes_analysis.append({
            **class_info,
            'analysis': analysis
        })
        
        # API 제한 방지를 위한 대기
        if i < len(classes):
            import time
            time.sleep(1)
    
    print("\n📝 문서 생성을 시작합니다...")
    
    # 3개 문서 생성 (MD 파일 없으면 생성, 있으면 추가)
    documents = [
        ("API_DOCS.md", create_api_docs),
        ("ARCHITECTURE.md", create_architecture_docs),
        ("README.md", create_readme_docs)
    ]
    
    for filename, create_func in documents:
        print(f"\n📄 {filename} 처리 중...")
        content = create_func(classes_analysis)
        
        if os.path.exists(filename):
            # 파일이 있으면 내용 추가 (덮어쓰기 아님)
            with open(filename, 'a', encoding='utf-8') as f:
                f.write(f"\n\n---\n\n## 업데이트: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
                f.write(content)
            print(f"✅ {filename}에 내용을 추가했습니다. (쌓아가기)")
        else:
            # 파일이 없으면 새로 생성
            with open(filename, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"✅ {filename}을 새로 생성했습니다. (첫 생성)")
        
        # API 제한 방지를 위한 대기
        import time
        time.sleep(2)
    
    print("\n🎉 모든 문서 생성이 완료되었습니다!")
    print("📁 생성된 파일들:")
    for filename, create_func in documents:
        if os.path.exists(filename):
            print(f"  - {filename}")

if __name__ == "__main__":
    main()
