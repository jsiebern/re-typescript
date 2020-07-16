import { Project, Node } from 'ts-morph';

const project = new Project({
  useInMemoryFileSystem: true,
  compilerOptions: {
    noEmitOnError: false,
  },
});

const file = project.createSourceFile(
  'test.d.ts',
  `

interface I_b { field: string }
interface I_a extends I_b {}

`
);

const walk = (node: Node) => {
  if (Node.isInterfaceDeclaration(node)) {
    //   node.getProject().getTypeChecker().
    console.log(
      node.getName(),
      node
        .getExtends()?.[0]
        ?.getType()
        .getSymbol()
        ?.getMembers()[0]
        .getValueDeclaration()
        ?.getKindName()

      //   console.log(node.getType()?.getSymbol()?.getDeclaredType().),
      //   node.getExtends()?.[0]?.getTypeArguments()
    );
    // console.log(node.getIndexTypeNode().getKindName());
    // console.log(node.getType().getSymbol()?.getDeclarations()[0].getKindName());
  } else {
    node.getChildren().forEach((n) => walk(n));
  }
};

walk(file);
