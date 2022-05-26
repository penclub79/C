Union(공용체)

Union과 Struct와 다른점

1. 구조체가 아닌 공용체이며, 공용체에 선언한 타입의 가장 큰 사이즈로 메모리 공간이 생긴다.
	예를들어)
		union UBuffer
		{
			int				iValue;
			unsigned char*	pszBuffer;
			long long		lIValue;
		};
	위 공용체에서 int에서부터 4바이트, 4바이트, 8바이트가 선언됐는데, 8바이트인 long long이 사이즈가 제일 큼으로
	각각 3개 변수 중 하나를 사용할 때 8바이트 메모리 공간이 생긴다.


2. iValue는 int이지만 char[]을 할당해서 사용할 수 있다.
자세히 설명하자면, iValue = new char[1024]; 이런 형태로 할당이 가능하다. 
공용체 안에 변수명은 라벨이라고 생각한다.