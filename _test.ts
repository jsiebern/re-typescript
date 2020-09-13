import { Project, Node, Type, ts, createWrappedNode } from 'ts-morph';
// import * as c from '@ts-morph/common';

const project = new Project({
  useInMemoryFileSystem: true,
  compilerOptions: {
    noEmitOnError: false,
  },
});



const file = project.createSourceFile(
  'test.d.ts',
  `
  type BoxedValue<T> = { value: T };
type BoxedArray<T> = { array: T[] };
type Boxed<T> = T extends any[] ? BoxedArray<T[number]> : BoxedValue<T>;

type T1 = Boxed<string>;
type T2 = Boxed<number[]>;
type T3 = Boxed<string | number[]>;
`);

file.forEachChild(child => {
  // if (child.getType().getFlags() & ts.TypeFlags.IndexedAccess) {
  //   console.log(child.getType().getText());
  //   child.getType().getBaseTypes()
  // }
  // if (Node.isConditionalTypeNode(child)) {
  // }
  if (Node.isTypeAliasDeclaration(child)) {
    const tn = child.getTypeNode();
    if (Node.isTypeReferenceNode(tn)) {
      const symbol = tn.getType().getAliasSymbol();
      const args = tn.getType().getAliasTypeArguments();
      // args.forEach(a => {a.get})
      const name = symbol.getName();
      const dec = symbol.getDeclarations()[0];
      if (Node.isTypeAliasDeclaration(dec)) {
        console.log(dec.getTypeParameters().map(t => t.getName()));
        console.log(name);
      }


      // console.log(tn.getType().getSymbol())
      // const t = tn.getType().compilerType;
      // const newNode = createWrappedNode(
      //   project
      //     .getTypeChecker()
      //     .compilerObject
      //     .typeToTypeNode(t, child.compilerNode),
      //   { typeChecker: project.getTypeChecker().compilerObject, sourceFile: file.compilerNode }
      // );
      // console.log(newNode.print())

      // if (tn.getType().getAliasSymbol()) {
      //   console.log('Y')
      // }
    }
    // const nam = child.getNameNode();
    //   console.log(tn.getType().getSymbol().getName());
    //   // ts.createTypeReferenceNode()
    //   const tt = child.getType();

    //   const n = child.getNameNode();
    // console.log(child.getTypeNode().getKindName())
    // console.log(n.getSymbol().getDeclarations().forEach(dec => console.log(dec.getText())));
    // console.log(t.getFlags() & ts.TypeFlags.Conditional);
    // const n = child.getTypeNode();
    // if (Node.isTypeReferenceNode(n)) {
    //   console.log(project.getTypeChecker())
    //   console.log(project.getTypeChecker().getApparentType(n.getType()).getText())
    //   // const d = n.getTypeName().getSymbol().getDeclarations()[0];
    //   // // @ts-ignore
    //   // const tp = d.getTypeParameters();
    //   // console.log(tp)
    // }
    // console.log(child.getTypeNode().getKindName())
    // console.log(child.getTypeNode().getText())
    console.log('---')
  }
  // console.log(Node.isConditionalTypeNode(child))
  // console.log(child.getKindName())
  // console.log(child.getText() + ' - ' + child.getType().getText())
});

console.log('\n -------------------------- \n');