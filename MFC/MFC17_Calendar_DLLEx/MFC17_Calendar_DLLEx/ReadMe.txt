[ DLL의 링크 ]

DLL을 링크하는 방법은 Implicit 링킹과 Explicit 링킹 두 가지 방법이 있다.
Implicit : 프로그램이 시작되면서 해당 DLL을 바로 로드하는 방법으로 DLL을 만들고 난 후 세 가지 파일을 이용하여 메인 프로그램을 작성한다.

※ 세 가지 파일 ※
DLL 파일, LIB 파일, .H 확장자 가진 헤더파일

Explicit : 세 가지 파일이 필요 없이 단순하게 DLL을 이용하여 로드할 수 있는 방법이다.

★ 해당 프로그램은 Implicit 링킹을 통한 DLL을 작성.