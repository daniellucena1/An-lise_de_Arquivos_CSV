# Contexto

O Governo Federal precisa de um sistema para consolidar os dados de novos casos de uma
epidemia no país. Estes dados são enviados diariamente pelas secretarias estaduais de saúde em arquivos
CSV e um sistema precisar consolidar uma base com os dados nacionais. Este é o seu trabalho.
Os arquivos CSV enviados pelas secretarias estaduais seguem o seguinte formato: 

`DataRegistro;CodCidade;CodEstado;NovosCasos`

Onde:

1. `DataRegistro` : é um inteiro longo que representa um instante de tempo no formato EpochTime
2. `CodCidade` e `CodEstado` : são inteiros que representam, respectivamente, o código da Cidade e do Estado aos quais a linha se refere
3. `NovosCasos` : representa o número de novos casos registrados na data informada no campo `DataRegistro`.

# O sistema é capaz de:

1. Realizar a carga dos arquivos CSV com novos casos e consolidar os dados de todas as cargas em um outro arquivo CSV. Os arquivos de carga contém um número indeterminado de linhas.
2. Os arquivos consolidados gerados pelo sistema devem seguir o seguinte formato: `CodCidade;CodEstado;TotalCasos;UltimaAtualizacao`
3. Gerar relatório de casos por ESTADO. O(A) usuário(a) irá informar o código de um estado da federação e, caso existam dados para este estado, o sistema irá gerar um relatório no formato abaixo: <br>
![image](https://github.com/daniellucena1/Analise_de_Arquivos_CSV/assets/115027219/eb19baca-4f0d-41aa-94c4-2f6bc8bd8795)
